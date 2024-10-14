/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:13 by mott              #+#    #+#             */
/*   Updated: 2024/10/14 20:11:01 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// int socket(int domain, int type, int protocol);
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
Server::Server(int port) : _port(port) {
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		throw std::runtime_error("socket(): " + std::string(strerror(errno)));
	}

	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		throw std::runtime_error("setsockopt(): " + std::string(strerror(errno)));
	}

	int flags = fcntl(_server_fd, F_GETFL, 0);
	fcntl(_server_fd, F_SETFL, flags | O_NONBLOCK);

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	socklen_t server_addr_size = sizeof(server_addr);

	if (bind(_server_fd, (struct sockaddr*)&server_addr, server_addr_size) == -1) {
		throw std::runtime_error("bind(): " + std::string(strerror(errno)));
	}

	if (listen(_server_fd, SOMAXCONN) == -1) {
		throw std::runtime_error("listen(): " + std::string(strerror(errno)));
	}

	_epoll.add_fd(_server_fd, EPOLLIN);
}

Server::~Server() {
	if (_server_fd != -1) {
		close(_server_fd);
	}
}

void Server::start() {

	struct epoll_event events[MAX_EVENTS];

	while (true) {
		std::cout << "Waiting for connection..." << std::endl;
		int nfds = _epoll.wait(events, MAX_EVENTS, 2000);

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == _server_fd) {
				handle_new_connection();
			}
			else {
				// std::cout << "handle_client_request fd: " << events[i].data.fd << std::endl;
				handle_client_request(events[i].data.fd);
			}
		}
	}
}

// int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen);
void Server::handle_new_connection() {
	int client_fd = accept(_server_fd, nullptr, nullptr);
	if (client_fd == -1) {
		throw std::runtime_error("accept(): " + std::string(strerror(errno)));
	}
	// std::cout << client_fd << std::endl;
	std::cout << "New client connected" << std::endl;

	int flags = fcntl(client_fd, F_GETFL, 0);
	fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

	// int flags = fcntl(client_fd, F_GETFL, 0);
	// if (flags == -1) {
	// 	throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
	// }
	// if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
	// 	throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
	// }

	_epoll.add_fd(client_fd, EPOLLIN | EPOLLOUT);
	// _epoll.add_fd(client_fd, EPOLLIN);
			// sleep(1);
}

void Server::handle_client_request(int client_fd) {
	Client client(client_fd);
	// std::vector<char> buffer(1024);
	std::cout << "hallo" << std::endl;
	// ssize_t nbytes = client.read_request(buffer);
	// ssize_t nbytes = client.read_request(_buffer);

	ssize_t nbytes;
	do {
		nbytes = read(client_fd, _buffer, sizeof(_buffer));
			std::cout << "errno: " << errno << std::endl;
			std::cout << std::string(strerror(errno)) << std::endl;
	// } while (nbytes == -1 && errno == EAGAIN && errno == EWOULDBLOCK);
	} while (nbytes == -1 && errno == EAGAIN);



	if (nbytes <= 0) {
		// std::cout << "Closing connection" << std::endl;
		// close(client_fd);
	}
	else {
		std::cout << "Request received:" << std::endl;
		std::cout << YELLOW << _buffer << RESET << std::endl;

		// HttpRequest request;
		// request.parse(_buffer);
		// RequestHandler handler;
		// std::string _http_response = handler.handleRequest(request)

		client.send_response(_http_response);
		std::cout << "Response sent!" << std::endl;
	}
}
