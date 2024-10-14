/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:00 by mott              #+#    #+#             */
/*   Updated: 2024/10/12 17:53:41 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include "Client.hpp"
#include "Epoll.hpp"
#include "external_functions.hpp"

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

class Server {
	public:
		Server(int port);
		~Server();

		Server() = delete;
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		void start();

	private:
		void handle_new_connection();
		void handle_client_request(int client_fd);

		int _server_fd;
		int _port;
		Epoll _epoll;
		char _buffer[BUFFER_SIZE];
		const std::string _http_response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 12\r\n"
			"\r\n"
			"Hello World!";
};

#endif // SERVER_H
