/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/17 23:48:54 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

// 1. Read in Config file
// 2. setup
// 3. start up loop

/// @brief Webserver Constructor -> Parses and prints config data and sets running to false
/// @param conf_file Saves data structures and values read in from the config file
WebServer::WebServer(const std::string &conf_file) : _config(Config::parse(conf_file)), _running(false)
{
	_config.print();
}

void WebServer::setupListeners()
{
	for (const auto& server : _config.getServerConfs())
	{
		std::vector<int> ports = {server._port};
		for (const auto& route : server._routes)
		{
			if (route.second._port.has_value() && route.second._port.value() != 0 && route.second._port.value() != server._port)
				ports.push_back(route.second._port.value());
		}

		for (const auto& port : ports)
		{
			std::string server_key = server._hostname + ":" + std::to_string(port);

			int fd = createNonBlockingSocket();
			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(port);

			if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
			{
				close(fd);
				throw std::runtime_error("Failed to bind socket");
			}

			if (listen(fd, SOMAXCONN) < 0)
			{
				close(fd);
				throw std::runtime_error("Failed to listen on socket");
			}
			_server_listeners[server_key].push_back(fd);
			_event_loop.addFd(fd, EPOLLIN_FLAG);
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
	_event_loop.addFd(client_fd, EPOLLIN_FLAG);
}

int WebServer::createNonBlockingSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		throw std::runtime_error("Couldn't open socket");
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	return fd;
}

void WebServer::start()
{
	if (_server_listeners.empty())
		throw std::runtime_error("No listeners set up.");
	_running = true;
	runLoop();
}

void WebServer::stop()
{
	_running = false;
	for (auto [_, fds] : _server_listeners)
	{
		for (int fd : fds)
		{
			close(fd);
			_event_loop.removeFd(fd);
		}
	}
	_server_listeners.clear();
}

void WebServer::runLoop()
{
	while (_running)
	{
		std::cout << "waiting for connection" << std::endl;
		auto events = _event_loop.wait();
		for (const auto& [fd, event] : events) {
			if (event & EPOLLERR_FLAG || event & EPOLLHUP_FLAG)
			{
				close(fd);
				_event_loop.removeFd(fd);
			}
			else if (event & EPOLLIN_FLAG)
			{
				bool is_listener = false;
				for (const auto& [_, fds] : _server_listeners)
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
		}
	}
}

/// @brief initializes epoll_fd and sets up the listening socket fds. Also loads the error pages into the WebServer class
void WebServer::initialize()
{
	setupListeners();
	loadErrorPages();
	request_handler = std::make_unique<RequestHandler>(_error_pages, _config);
}

void WebServer::loadErrorPages()
{
	for (const auto &server : _config.getServerConfs())
	{
		if (!server._default_error_pages.empty())
		{
			std::ifstream file(server._default_error_pages);
			std::string line;
			while (std::getline(file, line))
			{
				std::istringstream iss(line);
				int error_code;
				std::string page_path;
				if (iss >> error_code >> page_path)
					_error_pages[error_code] = page_path;
			}
		}
	}
}

void WebServer::handleClientRequest(int client_fd)
{
	// (void)client_fd;
	Client client(client_fd);

	std::cout << "request from " << client_fd << std::endl;
	if(!client.read_request())
	{
		request_handler->serveErrorPage(client, 404);
		return;
	}
	// request_handler->handleRequest(client);

	std::cout << "response to " << client_fd << std::endl;
	client.send_response();
}