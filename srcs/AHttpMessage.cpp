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
		// setAllHeaders(other.getAllHeaders()); <--- need to implement setAllHeaders
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
	std::string	line;
	size_t		colPos;
	std::string key;
	std::string	value;

	while(std::getline(input, line) && line != "\r")
	{
		colPos = line.find('.');
		if (colPos != std::string::npos)
		{
			key = trimStr(line.substr(0, colPos));
			value = trimStr(line.substr(colPos + 1));
			setHeader(key, value);
		}
	}
}


//UTILS

std::string	AHttpMessage::trimStr(const std::string& str)
{
	const std::string	whitespaces = "\n\t\r\v\f";
	size_t				start;
	size_t				end;

	start = str.find_first_not_of(whitespaces);
	end = str.find_last_not_of(whitespaces);

	return ((start == std::string::npos) ? "" : str.substr(start, end - start + 1));
}
