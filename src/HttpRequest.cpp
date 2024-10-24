/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/24 02:09:16 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{}

HttpRequest::HttpRequest(const HttpRequest &other) : AHttpMessage()
{
	*this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest &other)
{
	if (this != &other)
	{
		AHttpMessage::operator=(other);
		setMethod(other.getMethod());
		setUri(other.getUri());
	}
	return (*this);
}

HttpRequest::~HttpRequest()
{}

const std::string& HttpRequest::getMethod() const
{
	return (_method);
}

const std::string&	HttpRequest::getUri() const
{
	return (_uri);
}

void	HttpRequest::setMethod(const std::string& method)
{
	_method = method;
}
void	HttpRequest::setUri(const std::string& uri)
{
	_uri = uri;
}

bool	HttpRequest::parse(const std::string& rawmsg)
{
	std::istringstream	input(rawmsg);
	std::string			line;

	if (!std::getline(input, line))
		return (false);

	std::istringstream	reqLine(line);
	std::string	method;
	std::string	vers;
	std::string	uri;

	if (!(reqLine >> method >> uri >> vers))
		return (false);

	setMethod(trimStr(method));
	setUri(trimStr(uri));
	setHttpVersion(trimStr(vers));

	parseHeader(input);

	std::string body((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	setBody(trimStr(body));
	return (true);
}
