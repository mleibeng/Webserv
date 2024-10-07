/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:42 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 15:56:43 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

HttpMessage::HttpMessage(const HttpMessage &other)
{
	// std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

HttpMessage& HttpMessage::operator=(const HttpMessage &other)
{
	if (this != &other)
	{
		// std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

HttpMessage::~HttpMessage()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

void	HttpMessage::setHttpVersion(const std::string& vers)
{
	_httpVersion = vers;
}
void	HttpMessage::setHeader(const std::string& key, const std::string& val)
{
	_header[key] = val;
}
void	HttpMessage::setBody(const std::string& body)
{
	_body = body;
	setHeader("Content-Length", std::to_string(body.length()));
}

std::string		HttpMessage::getHttpVersion() const
{
	return (_httpVersion);
}
std::string		HttpMessage::getHeader(const std::string& key) const
{
	auto iter = (_header.find(key));
	return (iter != _header.end() ? iter->second : "");
}
std::string		HttpMessage::getBody() const
{
	return (_body);
}
