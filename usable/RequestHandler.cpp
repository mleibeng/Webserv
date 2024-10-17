/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/17 21:55:20 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

RequestHandler::~RequestHandler()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

HttpResponse		RequestHandler::handleRequest(const HttpRequest& request)
{
	switch(request.getMethod())
	{
		case Method::GET:
			return (handleGetRequest(request));
		case Method::POST:
			return (handlePostRequest(request));
		case Method::DELETE:
			return (handleDeleteRequest(request));
		default:
		{
			//Error::getErrorPage(501);
		}
	}
}

HttpResponse		handleGetRequest(const HttpRequest& request)
{
	
}
HttpResponse		handlePostRequest(const HttpRequest& request)
{

}

HttpResponse		handleDeleteRequest(const HttpRequest& request)
{

}

