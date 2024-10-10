/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:18:42 by mott              #+#    #+#             */
/*   Updated: 2024/10/10 20:48:04 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>			// std::memset
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define RESET  "\033[0m"
#define YELLOW "\033[33m"

void set_nonblocking(int sockfd) {
	int flags;

	flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
	int server_fd;
	int client_fd;
	int epoll_fd;
	struct sockaddr_in server_addr;
	socklen_t server_addr_size;
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];
	int nfds;
	char buffer[BUFFER_SIZE];
	const char* http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 12\r\n"
		"\r\n"
		"Hello World!";
	ssize_t count;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	set_nonblocking(server_fd);

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	server_addr_size = sizeof(server_addr);

	bind(server_fd, (struct sockaddr*)&server_addr, server_addr_size);

	listen(server_fd, SOMAXCONN);

	// epoll_fd = epoll_create(0);
	epoll_fd = epoll_create1(0);

	event.events = EPOLLIN;
	event.data.fd = server_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

	while (true) {
		std::cout << "Wait for connection" << std::endl;
		// nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 2000);

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == server_fd) {
				client_fd = accept(server_fd, NULL, NULL);
				set_nonblocking(client_fd);

				event.events = EPOLLIN | EPOLLOUT;
				event.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
			}
			else {
				count = read(events[i].data.fd, buffer, sizeof(buffer));
				if (count <= 0) {
					close(events[i].data.fd);
				}
				else {
					std::cout << "Request received:" << std::endl;
					std::cout << buffer << std::endl;

					write(client_fd, http_response, std::strlen(http_response));
				}
			}
		}
	}
	return 0;
}

// int epoll_create(int size);
// int epoll_create1(int flags);
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *_Nullable event);
// int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

// int epfd = file descriptor to epoll instance
// int op =	EPOLL_CTL_ADD
// 			EPOLL_CTL_MOD
// 			EPOLL_CTL_DEL

// struct epoll_event {
// 	uint32_t      events;  /* Epoll events */
// 	epoll_data_t  data;    /* User data variable */
// };

// union epoll_data {
// 	void     *ptr;
// 	int       fd;
// 	uint32_t  u32;
// 	uint64_t  u64;
// };

// typedef union epoll_data  epoll_data_t;
