/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:58 by mott              #+#    #+#             */
/*   Updated: 2024/10/13 20:08:18 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Server.hpp"
#include "Client.hpp"

#include <iostream>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>

#include <sys/epoll.h>

#define MAX_EVENTS	100
#define TIMEOUT		1000

#define DEFAULT		"\033[0m"
#define RED			"\033[31m"
#define YELLOW		"\033[33m"

class EventLoop {
	public:
		EventLoop(int port);
		~EventLoop();

		EventLoop() = delete;
		EventLoop(const EventLoop& other) = delete;
		EventLoop& operator=(const EventLoop& other) = delete;

		void add_server_socket(const Server& server);
		void add_client_socket(int client_fd);
		void start();

	private:
		void handle_server_event();
		void handle_client_event(int client_fd);

		int _epoll_fd;
		struct epoll_event _events[MAX_EVENTS];
		Server _server;
};

#endif // EVENTLOOP_H
