/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/25 17:06:52 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

//utils
std::string getTime()
{
	std::time_t now = std::time(nullptr);
	std::string form_time = std::ctime(&now);
	form_time.erase(form_time.find_first_not_of("\n") + 1);
	return form_time;
}

HttpRequest::HttpRequest()
{
}

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
		_uri = other.getUri();
		setQuery(other.getQuery());
	}
	return (*this);
}

HttpRequest::~HttpRequest()
{
}

const std::string& HttpRequest::getMethod() const
{
	return (_method);
}

const std::string&	HttpRequest::getUri() const
{
	return (_uri);
}

const std::string& HttpRequest::getQuery() const
{
	return (_query);
}

void	HttpRequest::setMethod(const std::string& method)
{
	_method = method;
}

void	HttpRequest::setQuery(const std::string& uri)
{
	size_t query_point = uri.find('?');
	if (query_point != std::string::npos)
	{
		_query = uri.substr(query_point + 1);
		_uri = uri.substr(0, query_point);
	}
	else
	{
		_query.clear();
		_uri = uri;
	}
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
	setQuery(trimStr(uri)); // now splits query from uri and sets both.
	setHttpVersion(trimStr(vers));

	parseHeader(input);

	std::string body((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	setBody(trimStr(body));

	parseCookies();
	return (true);
}
