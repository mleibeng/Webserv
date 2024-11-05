/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/06 00:39:58 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

// 1. Read in Config file
// 2. setup
// 3. start up loop

/// @brief Webserver Constructor -> Parses and prints config data and sets running to false
/// @param conf_file Saves data structures and values read in from the config file
WebServer::WebServer(const std::string &conf_file) : config(Config::parse(conf_file)), running(false)
{
	// config.print();
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
			std::string server_key = server.hostname + ":" + std::to_string(port);

			int fd = createNonBlockingSocket();

			int opt = 1;
			if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
				std::cerr << RED << "setsockopt(): " << strerror(errno) << DEFAULT << std::endl;

			struct addrinfo hints, *res;
			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			int status = getaddrinfo(server.hostname.c_str(), std::to_string(port).c_str(), &hints, &res);
			if (status != 0)
			{
				std::cerr << "getaddrinfo failed: " << gai_strerror(status) << std::endl;
				close(fd);
				throw std::runtime_error("Failed to resolve hostname: " + server.hostname);
			}

 			if (bind(fd, res->ai_addr, res->ai_addrlen) < 0)
			{
				std::cerr << "Failed to bind to " << server.hostname << ":" << port
							<< " - " << strerror(errno) << std::endl;
				close(fd);
				freeaddrinfo(res);
				throw std::runtime_error("Failed to bind socket");
			}

			freeaddrinfo(res);

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
	event_loop.addFd(client_fd, EPOLLIN_FLAG);
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
void WebServer::runLoop()
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
			else if (event & EPOLLIN_FLAG)
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
					handleClientRequest(fd, *request_handler);
			}
		}
	}
}

/// @brief initializes epoll_fd and sets up the listening socket fds. Also loads the error pages into the WebServer class
void WebServer::initialize()
{
	setupListeners();
	request_handler = std::make_unique<RequestHandler>(config);
}

WebServer::ClientInfo::ClientInfo(int fd) : client(std::make_unique<Client>(fd)), last_active(std::time(nullptr))
{
}

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
void WebServer::handleClientRequest(int client_fd, RequestHandler& handler)
{
	auto it = active_clients.find(client_fd);
	if (it == active_clients.end())
	{
		active_clients.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(client_fd),
			std::forward_as_tuple(client_fd)
		);
		it = active_clients.find(client_fd);
	}

	it->second.last_active = std::time(nullptr);

	Client& client = *it->second.client;

	client.setBuffer(std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_HEADER_SIZE)) + std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_BODY_SIZE)));
	// std::cout << "request from " << client_fd << std::endl;
	if (client.read_request() == -1)
		return handler.serveErrorPage(client, 400);
	handler.handleRequest(client);
}
