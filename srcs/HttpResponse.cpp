/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:50 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 15:56:51 by fwahl            ###   ########.fr       */
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
