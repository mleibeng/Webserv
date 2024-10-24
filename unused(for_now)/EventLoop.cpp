/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:06 by mott              #+#    #+#             */
/*   Updated: 2024/10/13 20:07:30 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"

EventLoop::EventLoop(int port) : _server(Server(port)) {
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1) {
		std::cerr << RED << "epoll_create(): " << strerror(errno) << DEFAULT << std::endl;
	}
	// else {
	// 	std::cout << "epoll_create()" << std::endl;
	// }
	add_server_socket(_server);
}

EventLoop::~EventLoop() {
	close(_epoll_fd);
}

void EventLoop::add_server_socket(const Server& server) {
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server.get_fd();

	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, server.get_fd(), &event) == -1) {
		std::cerr << RED << "epoll_ctl(): " << strerror(errno) << DEFAULT << std::endl;
	}
	// else {
	// 	std::cout << "epoll_ctl()" << std::endl;
	// }
}

void EventLoop::add_client_socket(int client_fd) {
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = client_fd;

	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
		std::cerr << RED << "epoll_ctl(): " << strerror(errno) << DEFAULT << std::endl;
	}
	// else {
	// 	std::cout << "epoll_ctl()" << std::endl;
	// }
}

void EventLoop::start() {
	int nfds;

	while (true) {
		nfds = epoll_wait(_epoll_fd, _events, MAX_EVENTS, TIMEOUT);
		if (nfds == -1) {
			std::cerr << RED << "epoll_wait(): " << strerror(errno) << DEFAULT << std::endl;
		}
		else {
			std::cout << "epoll_wait()" << std::endl;
		}

		for (int i = 0; i < nfds; i++) {
			if (_events[i].data.fd == _server.get_fd()) {
				handle_server_event();
			}
			else {
				handle_client_event(_events[i].data.fd);
			}
		}
	}
}

void EventLoop::handle_server_event() {
	int client_fd = _server.accept_client();
	add_client_socket(client_fd);
}

void EventLoop::handle_client_event(int client_fd) {
	Client client(client_fd);

	std::cout << "request from " << client_fd << std::endl;
	client.read_request();

	std::cout << "response to " << client_fd << std::endl;
	client.send_response();
}
