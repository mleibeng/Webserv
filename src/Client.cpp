/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/11/07 03:44:26 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// -----------------------------------------------------------------------------

Client::Client(int client_fd, const Config& config) : _client_fd(client_fd), _config(config), _buffersize(std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_HEADER_SIZE))), redirect_count(0), _route(nullptr), _keep_alive(true)
{}

Client::~Client()
{
	close(_client_fd);
}

void Client::setBuffer(size_t buffersize)
{
	_buffersize = buffersize;
}

ssize_t Client::processChunk(const std::string& chunky)
{
	try
	{
		switch (_request.getState())
		{
			case HttpRequest::State::R_HEADER:
				if(!_request.parseHeaderChunk(chunky))
					return -1;
				break;
			case HttpRequest::State::R_BODY:
				if (!_route)
				{
					_request.setState(HttpRequest::State::ERROR);
					return -1;
				}
				if (!_request.parseBodyChunk(chunky))
					return -1;
				break;
			case HttpRequest::State::COMPLETE:
			case HttpRequest::State::ROUTING:
			case HttpRequest::State::ERROR:
				return 0;
		}
		return chunky.size();
	}
	catch (const std::exception& e)
	{
		_request.setState(HttpRequest::State::ERROR);
		return -1;
	}
}

/// @brief reads in the clientside data sent from the webbrowser
/// @return returns length of request or -1 in case of error
ssize_t Client::read_request()
{
	char buffer[_buffersize];
	std::string request;

	ssize_t nbytes = read(_client_fd, buffer, _buffersize);

	if (nbytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		return -1;
	}

	if (nbytes == 0)
	return 0;

	return processChunk(std::string(buffer, nbytes));
}

bool Client::isFileUpload() const
{
	const std::string& content_type = _request.getHeader("Content-Type");
	return content_type.find("multipart/form-data") != std::string::npos;
}

bool Client::needsRouteResolution() const
{
	return _request.getState() == HttpRequest::State::ROUTING;
}

void Client::setRoute(const RouteConf* route)
{
	_route = route;
	if (isFileUpload())
		_request.initUpload(*route);
	_request.setState(HttpRequest::State::R_BODY);
}

int Client::getNumRedirects() const
{
	return (redirect_count);
}

void Client::increaseRedirectCount()
{
	++redirect_count;
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
