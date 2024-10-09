/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 12:49:13 by mott              #+#    #+#             */
/*   Updated: 2024/10/09 13:07:09 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

HTTPServer::HTTPServer() : port(8080), backlog(3) {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		handle_error("socket");
	}

	std::memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(port);
	my_addr_size = sizeof(my_addr);

	if (bind(server_fd, (struct sockaddr*)&my_addr, my_addr_size) == -1) {
		handle_error("bind");
	}

	if (listen(server_fd, backlog) == -1) {
		handle_error("listen");
	}

}

HTTPServer::~HTTPServer() {
	close(server_fd);
}

void HTTPServer::run() {
	std::cout << "Wait for connection on port " << port << std::endl;

	client_fd = accept(server_fd, (struct sockaddr*)&my_addr, &my_addr_size);
	if (client_fd == -1) {
		handle_error("accept");
	}

	const char* http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 12\r\n"
		"\r\n"
		"Hello World!";
	send(client_fd, http_response, strlen(http_response), 0);

	std::cout << "HTTP response sent!" << std::endl;

	close(client_fd);
}

void HTTPServer::handle_error(const char* msg) {
	perror(msg);
	exit(1);
}
