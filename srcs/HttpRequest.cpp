/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 19:36:49 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

HttpRequest::HttpRequest(const HttpRequest &other)
{
	// std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest &other)
{
	if (this != &other)
	{
		// std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

HttpRequest::~HttpRequest()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

//GETTERS

std::string HttpRequest::getMethod() const
{
	return (_method);
}

std::string HttpRequest::getPath() const
{
	return (_path_name);
}

std::string HttpRequest::getQuery() const
{
	return (_query_name);
}

//SETTERS
void	HttpRequest::setMethod(Method method)
{
	switch (method)
	{
		case Method::GET:
			_method = "GET";
			break;
		case Method::POST:
			_method = "POST";
			break;
		case Method::DELETE:
			_method = "DELETE";
			break;
	}
}
void	HttpRequest::setUri(const std::string& uri)
{
	_uri = uri;
}

//PARSE

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

	try
	{
		setMethod(strToMethod(method));
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << std::endl;
		return (false);
	}
	setUri(uri);
	setHttpVersion(vers);

	parseHeader(input);

	std::string body((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	setBody(body);
	return (true);
}


Method		HttpRequest::strToMethod(const std::string& method)
{
	if (method == "GET")
		return (Method::GET);
	else if (method == "POST")
		return (Method::POST);
	else if (method == "DELETE")
		return (Method::DELETE);
	else
		throw (InvalidMethodException());
}


const char*	HttpRequest::InvalidMethodException::what() const noexcept
{
	return ("Invalid Method");
}
