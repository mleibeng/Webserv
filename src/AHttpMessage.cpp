/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:42 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 18:37:31 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpMessage.hpp"

AHttpMessage::AHttpMessage()
{

}

AHttpMessage::AHttpMessage(const AHttpMessage &other)
{
	*this = other;
}

AHttpMessage& AHttpMessage::operator=(const AHttpMessage &other)
{
	if (this != &other)
	{
		setHttpVersion(other.getHttpVersion());
		setAllHeaders(other._header);
		setBody(other.getBody());
	}
	return (*this);
}

AHttpMessage::~AHttpMessage()
{
}

//SETTERS

void	AHttpMessage::setHttpVersion(const std::string& vers)
{
	_httpVersion = vers;
}

void	AHttpMessage::setBody(const std::string& body)
{
	_body = body;
	setHeader("Content-Length", std::to_string(body.length()));
}

void	AHttpMessage::setHeader(const std::string& key, const std::string& val)
{
	_header[key] = val;
}

void AHttpMessage::setAllHeaders(const std::map<std::string, std::string>& headers)
{
	_header.clear();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		_header[it->first] = it->second;
	}
}


//GETTERS

std::string		AHttpMessage::getHttpVersion() const
{
	return (_httpVersion);
}

std::string		AHttpMessage::getBody() const
{
	return (_body);
}

std::string		AHttpMessage::getHeader(const std::string& key) const
{
	auto iter = (_header.find(key));
	return (iter != _header.end() ? iter->second : "");
}

const std::map<std::string, std::string>&	AHttpMessage::getAllHeaders() const
{
	return (_header);
}

//PARSE

void	AHttpMessage::parseHeader(std::istringstream& input)
{
	std::string line;
	while (std::getline(input, line) && !line.empty() && line != "\r") {
		size_t separator = line.find(':');
		if (separator != std::string::npos) {
			std::string key = trimStr(line.substr(0, separator));
			std::string value = trimStr(line.substr(separator + 1));
			setHeader(key,value);
		}
	}
}


//UTILS

std::string	AHttpMessage::trimStr(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";

	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}
