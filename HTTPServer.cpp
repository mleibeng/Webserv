/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 12:49:13 by mott              #+#    #+#             */
/*   Updated: 2024/10/09 17:13:58 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

HTTPServer::HTTPServer() : port(8080), backlog(3) {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		throw std::runtime_error("socket(): " + std::string(strerror(errno)));
	}

	std::memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(port);
	my_addr_size = sizeof(my_addr);

	if (bind(server_fd, (struct sockaddr*)&my_addr, my_addr_size) == -1) {
		throw std::runtime_error("bind(): " + std::string(strerror(errno)));
	}

	if (listen(server_fd, backlog) == -1) {
		throw std::runtime_error("listen(): " + std::string(strerror(errno)));
	}
}

HTTPServer::~HTTPServer() {
	if (server_fd != -1) {
		close(server_fd);
	}
}

void HTTPServer::run() {
	std::cout << "Wait for connection on port " << port << std::endl;

	client_fd = accept(server_fd, (struct sockaddr*)&my_addr, &my_addr_size);
	if (client_fd == -1) {
		throw std::runtime_error("accept(): " + std::string(strerror(errno)));
	}

	const char* http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 12\r\n"
		"\r\n"
		"Hello World!";

	if (send(client_fd, http_response, strlen(http_response), 0) == -1) {
		throw std::runtime_error("send(): " + std::string(strerror(errno)));
	}

	std::cout << "HTTP response sent!" << std::endl;

	close(client_fd);
}
