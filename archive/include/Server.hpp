/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:00 by mott              #+#    #+#             */
/*   Updated: 2024/10/13 18:32:27 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define DEFAULT		"\033[0m"
#define RED			"\033[31m"
#define YELLOW		"\033[33m"

class Server {
	public:
		Server(int port);
		~Server();

		Server() = delete;
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		int accept_client();
		int get_fd() const;

	private:
		void set_nonblocking(int socket_fd);

		int _server_fd;
		int _port;
};

#endif // SERVER_H
