/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:50 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/08 01:00:41 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

HttpResponse::HttpResponse(const HttpResponse &other)
{
	// std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

HttpResponse& HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
		// std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

HttpResponse::~HttpResponse()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

//SETTERS

void HttpResponse::setStatus(StatusCode status)
{
	// change Status to input
	// and make check if it is even acceptable status as well?
}

//GETTERS

StatusCode	HttpResponse::getStatus() const
{
	return (_status);
}

bool	HttpResponse::parse(const std::string& rawmsg)
{
	std::istringstream	input(rawmsg);
	std::string			line;

	if (!std::getline(input, line))
		return (false);

	std::istringstream	status(line);
	std::string			vers;
	std::string			status_code;
	std::string			status_txt;

	if (!(status >> vers >> status_code >> std::ws))
		return (false);

	std::getline(status, status_txt);
	setHttpVersion(vers);
	int	code = std::stoi(status_code);
	setStatus(static_cast<StatusCode>(code));
	parseHeader(input);

	// parse body
}
