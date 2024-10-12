/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/10/12 17:56:39 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

class Client {
	public:
		Client(int client_fd);
		~Client();

		Client() = delete;
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;

		// ssize_t read_request(std::vector<char>& buffer);
		ssize_t read_request(char* buffer);
		ssize_t send_response(const std::string& response);

	private:
		int _client_fd;
};

#endif // CLIENT_H
