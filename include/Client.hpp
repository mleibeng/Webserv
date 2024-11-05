/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/11/03 21:59:55 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <cerrno>
#include <string>

#include <unistd.h>
#include <fcntl.h>

#include "HttpRequest.hpp"

#define DEFAULT		"\033[0m"
#define RED			"\033[31m"
#define YELLOW		"\033[33m"

class Client {
	public:
		Client(int client_fd);
		~Client();

		Client() = delete;
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;

		const HttpRequest& getRequest() const;
		const int& getFd() const;

		ssize_t read_request();
		ssize_t send_response(const std::string& response_string);

		int getNumRedirects() const;

		//incrementers
		void increaseRedirectCount();

	private:
		int _client_fd;
		HttpRequest _request;
		size_t redirect_count;
};

#endif // CLIENT_H
