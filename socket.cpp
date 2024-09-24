/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 15:41:48 by mott              #+#    #+#             */
/*   Updated: 2024/09/22 19:36:59 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// int socket(int domain, int type, int protocol);
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
// int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen);
// ssize_t send(int sockfd, const void buf[.len], size_t len, int flags);

// uint32_t htonl(uint32_t hostlong);
// uint16_t htons(uint16_t hostshort);
// uint32_t ntohl(uint32_t netlong);
// uint16_t ntohs(uint16_t netshort);

// struct sockaddr {
// 	sa_family_t     sa_family;      /* Address family */
// 	char            sa_data[];      /* Socket address */
// };

// struct sockaddr_in {
// 	sa_family_t     sin_family;     /* AF_INET */
// 	in_port_t       sin_port;       /* Port number */
// 	struct in_addr  sin_addr;       /* IPv4 address */
// };

// struct in_addr {
// 	in_addr_t s_addr;
// };

// AF_INET:		For IPv4 addresses (the most commonly used option).
// AF_INET6:		For IPv6 addresses.
// AF_UNIX:		For local communication between processes on the same machine.
// SOCK_STREAM:	Provides a sequenced, reliable, two-way connection-based byte stream (e.g., TCP).
// SOCK_DGRAM:		Provides a connectionless, unreliable datagram service (e.g., UDP).
// If domain =		AF_INET and type = SOCK_STREAM, then protocol = 0 will use TCP.
// If domain =		AF_INET and type = SOCK_DGRAM, then protocol = 0 will use UDP.

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>		// socket() + socket structs
#include <netinet/in.h>		// Internet domain sockets structs
#include <arpa/inet.h>		// htons()

static void handle_error(const char* msg) {
	perror(msg);
	exit(1);
}

int main() {
	int server_fd, client_fd;
	struct sockaddr_in my_addr;
	socklen_t my_addr_size;
	const int port = 8080;
	const int backlog = 3;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		handle_error("socket");
	}

	memset(&my_addr, 0, sizeof(my_addr));
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

	// Now we can accept incoming connections one at a time using accept().
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
	close(server_fd);

	return 0;
}

// Firefox -> http://localhost:8080
// Firefox -> http://127.0.0.1:8080
