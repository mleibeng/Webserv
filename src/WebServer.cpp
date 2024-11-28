/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/28 23:38:27 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

// 1. Read in Config file
// 2. setup
// 3. start up loop

/// @brief Webserver Constructor -> Parses and prints config data and sets running to false
/// @param conf_file Saves data structures and values read in from the config file
// event_loop(config)
WebServer::WebServer(const std::string &conf_file) : config(Config::parse(conf_file)), running(false)
{
	// config.print();
}

bool WebServer::portInUse(int port)
{
	for (const auto& [key, _] : server_listeners)
	{
		size_t pos = key.find(':');
		if (pos != std::string::npos)
		{
			int blocked_port = std::stoi(key.substr(pos + 1));
			if (blocked_port == port)
				return true;
		}
	}
	return false;
}

/// @brief set up ports to listen for connections on each server
void WebServer::setupListeners()
{
	for (const auto& server : config.getServerConfs())
	{
		std::vector<int> ports = {server.port};
		for (const auto& route : server.routes)
		{
			if (route.second.port.has_value() && route.second.port.value() != 0 && route.second.port.value() != server.port)
				ports.push_back(route.second.port.value());
		}

		for (const auto& port : ports)
		{
			if (portInUse(port))
			{
				std::cerr << "port: " << port << " already in use" << std::endl;
				continue;
			}
			std::string server_key = server.hostname + ":" + std::to_string(port);

			int fd = createNonBlockingSocket();

			int opt = 1;
			if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
				std::cerr << RED << "setsockopt(): " << strerror(errno) << DEFAULT << std::endl;

			struct addrinfo addr, *res;
			memset(&addr, 0, sizeof addr);
			addr.ai_family = AF_INET;
			addr.ai_socktype = SOCK_STREAM;
			addr.ai_flags = AI_PASSIVE;

			int status = getaddrinfo(server.hostname.c_str(), std::to_string(port).c_str(), &addr, &res);
			if (status != 0)
			{
				close(fd);
				throw std::runtime_error("Could not resolve hostname: " + server.hostname);
			}

			if (bind(fd, res->ai_addr, res->ai_addrlen) < 0)
			{
				std::cerr << "Failed to bind address to" << server.hostname << std::endl;
				close(fd);
				freeaddrinfo(res);
				throw std::runtime_error("Failed to bind socket");
			}

			freeaddrinfo(res);

			// struct sockaddr_in addr;
			// addr.sin_family = AF_INET;
			// addr.sin_addr.s_addr = INADDR_ANY;
			// addr.sin_port = htons(port);

			// if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
			// {
			// 	close(fd);
			// 	throw std::runtime_error("Failed to bind socket");
			// }

			if (listen(fd, SOMAXCONN) < 0)
			{
				close(fd);
				throw std::runtime_error("Failed to listen on socket");
			}
			server_listeners[server_key].push_back(fd);
			event_loop.addFd(fd, EPOLLIN_FLAG);
			std::cout << "Successfully bound and listening on " << server_key << std::endl;
		}
	}
}

/// @brief Stops the Server, cleaning up resources
WebServer::~WebServer()
{
	stop();
}

/// @brief Provides eventLoop functionality, renaming and splitting up of functionality in the future
void WebServer::acceptConnections(int fd)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			std::cerr << "Couldn't accept connection" << strerror(errno) << std::endl;
		return;
	}
	int flags = fcntl(client_fd, F_GETFL, 0);
	fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
	event_loop.addFd(client_fd, EPOLLIN_FLAG | EPOLLOUT_FLAG);
	//event_loop.addClient(client_fd, std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_HEADER_SIZE)));
}

/// @brief create and set sockets non blocking
/// @return integer socket fd
int WebServer::createNonBlockingSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		throw std::runtime_error("Couldn't open socket");
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	return fd;
}

/// @brief check for set up server listeners and start loop
void WebServer::start()
{
	if (server_listeners.empty())
		throw std::runtime_error("No listeners set up.");
	running = true;
	runLoop();
}

/// @brief stop loop and close necessary file descriptors
void WebServer::stop()
{
	running = false;
	for (auto [_, fds] : server_listeners)
	{
		for (int fd : fds)
		{
			close(fd);
			event_loop.removeFd(fd);
		}
	}
	server_listeners.clear();
	for (auto& [fd, _] : active_clients)
	{
		event_loop.removeFd(fd);
		close(fd);
	}
	active_clients.clear();
}

/// @brief server loop waiting for events to happen and process
void WebServer::runLoop() // maybe need to modify for more clear subject rules
{

	while (running)
	{
		std::cout << "waiting for connection" << std::endl;
		auto events = event_loop.wait(5000);
		if (!running)
			break;

		cleanInactiveClients();

		for (const auto& [fd, event] : events) {
			if (event & EPOLLERR_FLAG || event & EPOLLHUP_FLAG)
			{
				active_clients.erase(fd);
				close(fd);
				event_loop.removeFd(fd);
			}
			if (event & EPOLLIN_FLAG)
			{
				bool is_listener = false;
				for (const auto& [_, fds] : server_listeners)
				{
					if (std::find(fds.begin(), fds.end(), fd) != fds.end())
					{
						acceptConnections(fd);
						is_listener = true;
						break;
					}
				}
				if (!is_listener)
					handleClientRequest(fd);
			}
			if (event & EPOLLOUT_FLAG)
			{
				std::cout << fd << std::endl;
				auto it = active_clients.find(fd);
				if (it != active_clients.end() && it->second.client->hasResponse())
					it->second.client->send_response(it->second.client->getResponseString());
			}
		}
	}
}

RequestHandler& WebServer::getNextHandler()
{
	return *handler_pool[current_handler++ % handler_pool.size()];
}

/// @brief initializes epoll_fd and sets up the listening socket fds. Also loads the error pages into the WebServer class
void WebServer::initialize(size_t pool)
{
	setupListeners();
	for (size_t i = 0; i < pool; ++i)
		handler_pool.push_back(std::make_unique<RequestHandler>(config));
}

WebServer::ClientInfo::ClientInfo(int fd, const Config& config) : client(std::make_unique<Client>(fd, config)), last_active(std::time(nullptr))
{}

void WebServer::cleanInactiveClients()
{
	std::time_t current_time = std::time(nullptr);
	auto it = active_clients.begin();
	while (it != active_clients.end())
	{
		if (current_time - it->second.last_active > std::get<int>(config.getGlobalConf(GlobalConf::ConfigKey::TIMEOUT)))
		{
			std::cout << "Cleaning inactive clients" << it->first << std::endl;
			event_loop.removeFd(it->first);
			close(it->first);
			it = active_clients.erase(it);
		}
		else
			++it;
	}
}

/// @brief read request from client and either serve error or process it
/// @param client_fd client to process
/// @param handler handler instance for all processes
void WebServer::handleClientRequest(int client_fd)
{
	auto it = active_clients.find(client_fd);
	if (it == active_clients.end())
	{
		active_clients.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(client_fd),
			std::forward_as_tuple(client_fd, config)
		);
		it = active_clients.find(client_fd);
	}

	it->second.last_active = std::time(nullptr);

	Client& client = *it->second.client;

	// std::cout << "request from " << client_fd << std::endl;
	if (client.read_request() == -1) // should read in headers
	{
		getNextHandler().serveErrorPage(client, 400); // need to mod serverErrorPage!!!
		event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
		return;
	}
	// Logik zum finden der korrekten Route nach dem einlesen der header
	int error = 0;
	if ((error = client.setCourse()) && error != 0)
	{
		getNextHandler().serveErrorPage(client, error); // need to mod serverErrorPage!!!
		event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
		return;
	}

	const RouteConf* route = client.getRoute();
	if (route && route->redirect.has_value())
	{
		if (client.getNumRedirects() >= route->max_redirects)
		{
			getNextHandler().serveErrorPage(client, 508); // need to mod serverErrorPage!!!
			event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
			return;
		}

		getNextHandler().handleRedirect(*route, client);
		client.setRoute(nullptr);
		event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
		return;
	}

	if (!client.isMethodAllowed(*client.getRoute(), client.getRequest().getMethod()))
	{
		getNextHandler().serveErrorPage(client, 405); // need to mod serverErrorPage!!!
		event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
		return;
	}

	getNextHandler().handleRequest(client);
	event_loop.modifyFd(client_fd, EPOLLOUT_FLAG);
}
