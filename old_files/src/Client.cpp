/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/10/12 22:00:25 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int client_fd) : _client_fd(client_fd) {
	// int flags = fcntl(_client_fd, F_GETFL, 0);
	// fcntl(_client_fd, F_SETFL, flags | O_NONBLOCK);
}

Client::~Client() {
	if (_client_fd != -1) {
		close(_client_fd);
	}
}

// ssize_t read(int fd, void buf[.count], size_t count);
// ssize_t recv(int sockfd, void buf[.len], size_t len, int flags);
// ssize_t Client::read_request(std::vector<char>& buffer) {
ssize_t Client::read_request(char* buffer) {
	ssize_t nbytes;
	// std::cout << _client_fd << std::endl;
	// nbytes = read(_client_fd, buffer.data(), buffer.size());
	nbytes = read(_client_fd, buffer, sizeof(buffer));
	// nbytes = recv(_client_fd, buffer.data(), buffer.size(), 0);
	// if (nbytes == -1) {
		// throw std::runtime_error("read(): " + std::string(strerror(errno)));
		// throw std::runtime_error("recv(): " + std::string(strerror(errno)));
		// return 1;

		// if (errno == EAGAIN || errno == EWOULDBLOCK) {
		// 	std::cout << "errno: " << errno << std::endl;
		// 	std::cout << std::string(strerror(errno)) << std::endl;
			// Keine Daten zum Lesen
			// return 1;
	// 	}
	// 	else {
	// 		std::cerr << "Error reading from client: " << strerror(errno) << std::endl;
	// 		close(_client_fd);
	// 		return 0;
	// 	}
	// }

	return nbytes;
}

// ssize_t write(int fd, const void buf[.count], size_t count);
// ssize_t send(int sockfd, const void buf[.len], size_t len, int flags);
ssize_t Client::send_response(const std::string& response) {
	ssize_t nbytes;

	nbytes = write(_client_fd, response.c_str(), response.size());
	// nbytes = send(_client_fd, response.c_str(), response.size(), 0);

	return nbytes;
}
