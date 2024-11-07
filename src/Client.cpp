/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/11/07 05:07:30 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// -----------------------------------------------------------------------------

Client::Client(int client_fd, const Config& config) : _client_fd(client_fd), _config(config), _buffersize(std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_HEADER_SIZE))) , redirect_count(0), _keep_alive(true)
{}

Client::~Client()
{
	close(_client_fd);
}

void Client::setBuffer(size_t buffersize)
{
	_buffersize = buffersize;
}

/// @brief reads in the clientside data sent from the webbrowser
/// @return returns length of request or -1 in case of error
ssize_t Client::read_request()
{
	ssize_t nbytes;
	char buffer[_buffersize + 1];
	std::string request;

	// do {
		nbytes = read(_client_fd, buffer, sizeof(buffer));
	// } while (nbytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK));

	if (nbytes == -1)
		std::cerr << RED << "read(): " << strerror(errno) << DEFAULT << std::endl;
	else if (nbytes == 0)
		close(_client_fd);
	else if (nbytes == static_cast<ssize_t>(_buffersize + 1))
		return -1;
	else
	{
		request.assign(buffer, nbytes);
		std::cout << YELLOW << request << DEFAULT << std::endl;
		bool ok = _request.parse(request);
		if (!ok)
			return -1;
	}
	return nbytes;
}

int Client::getNumRedirects() const
{
	return (redirect_count);
}

void Client::increaseRedirectCount()
{
	redirect_count++;
}

/// @brief sends a response back to the client side
/// @param response_string std::string response to send
/// @return returns length of string sent or -1 in case of write error
ssize_t Client::send_response(const std::string& response_string)
{
	ssize_t nbytes;

	// std::cout << response_string << std::endl;
	nbytes = write(_client_fd, response_string.c_str(), response_string.size());
	if (nbytes == -1) {
		std::cerr << RED << "write(): " << strerror(errno) << DEFAULT << std::endl;
	}

	return nbytes;
}

/// @brief get fd of client
/// @return returns an integer
const int& Client::getFd() const
{
	return _client_fd;
}

/// @brief get parsed request as object
/// @return returns an HttpRequest object
const HttpRequest& Client::getRequest() const
{
	return _request;
}
