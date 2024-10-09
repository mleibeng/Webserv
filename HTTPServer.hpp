/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 12:49:16 by mott              #+#    #+#             */
/*   Updated: 2024/10/09 13:07:20 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <sys/socket.h>		// socket()
#include <netinet/in.h>		// sockaddr_in
#include <arpa/inet.h>		// htons()
#include <unistd.h>			// close()
#include <iostream>

class HTTPServer {
	public:
		HTTPServer();
		~HTTPServer();

		HTTPServer(const HTTPServer& other) = delete;
		HTTPServer& operator=(const HTTPServer& other) = delete;

		void run();

	private:
		static void handle_error(const char* msg);

		int server_fd;
		int client_fd;
		struct sockaddr_in my_addr;
		socklen_t my_addr_size;
		const int port;
		const int backlog;
};

#endif // HTTPSERVER_H
