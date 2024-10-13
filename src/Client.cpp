/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/10/13 19:14:50 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int client_fd) : _client_fd(client_fd) {
}

Client::~Client() {
	close(_client_fd);
}

ssize_t Client::read_request() {
	ssize_t nbytes;
	char buffer[BUFFER_SIZE];

	do {
		nbytes = read(_client_fd, buffer, sizeof(buffer));
	} while (nbytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK));

	if (nbytes == -1) {
		std::cerr << RED << "read(): " << strerror(errno) << DEFAULT << std::endl;
	}
	else if (nbytes == 0) {
		close(_client_fd);
	}
	else {
		_request.assign(buffer, nbytes);
		std::cout << YELLOW << _request << DEFAULT << std::endl;
	}

	return nbytes;
}

ssize_t Client::send_response() {
	ssize_t nbytes;

	nbytes = write(_client_fd, _response.c_str(), _response.size());
	if (nbytes == -1) {
		std::cerr << RED << "write(): " << strerror(errno) << DEFAULT << std::endl;
	}

	return nbytes;
}
