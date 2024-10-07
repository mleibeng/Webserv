/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 16:18:14 by fwahl            ###   ########.fr       */
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

Method HttpRequest::getMethod()
{
	return (_method);
}

std::string& HttpRequest::getPath()
{
	return (_path_name);
}

std::string& HttpRequest::getQuery()
{
	return (_query_name);
}