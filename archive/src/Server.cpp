/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:13 by mott              #+#    #+#             */
/*   Updated: 2024/10/13 18:40:31 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port) : _port(port) {
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		std::cerr << RED << "socket(): " << strerror(errno) << DEFAULT << std::endl;
	}

	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << RED << "setsockopt(): " << strerror(errno) << DEFAULT << std::endl;
	}

	set_nonblocking(_server_fd);

	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	socklen_t server_addr_size = sizeof(server_addr);

	if (bind(_server_fd, (struct sockaddr*)&server_addr, server_addr_size) == -1) {
		std::cerr << RED << "bind(): " << strerror(errno) << DEFAULT << std::endl;
	}

	if (listen(_server_fd, SOMAXCONN) == -1) {
		std::cerr << RED << "listen(): " << strerror(errno) << DEFAULT << std::endl;
	}
}

Server::~Server() {
	close(_server_fd);
}

int Server::accept_client() {
	int client_fd = accept(_server_fd, nullptr, nullptr);
	if (client_fd == -1) {
		std::cerr << RED << "accept(): " << strerror(errno) << DEFAULT << std::endl;
	}

	set_nonblocking(client_fd);

	return client_fd;
}

int Server::get_fd() const {
	return _server_fd;
}

void Server::set_nonblocking(int socket_fd) {
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
		std::cerr << RED << "fcntl(): " << strerror(errno) << DEFAULT << std::endl;
}
