#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include "HttpRequest.hpp"

#define BUFFER_SIZE	1024

#define DEFAULT		"\033[0m"
#define RED			"\033[31m"
#define YELLOW		"\033[33m"

class Client
{
	public:
		Client(int client_fd);
		~Client();

		Client() = delete;
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;

		ssize_t read_request();
		ssize_t send_response(const std::string& response);

		const std::string getRequest();

	private:
		int _client_fd;
		std::string _request;
		std::string _response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 12\r\n"
			"\r\n"
			"Hello World!";
};

#endif // CLIENT_H