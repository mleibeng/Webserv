/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/18 19:58:23 by fwahl            ###   ########.fr       */
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
			HttpResponse response;
			response.setStatus(StatusCode::NOT_IMPLEMENTED);
			// response.setBody(error.getErrorPage(501));
			response.setHeader("Content-Type", "html/txt");
			return (response);
		}
	}
}

HttpResponse		RequestHandler::handleGetRequest(const HttpRequest& request)
{
	HttpResponse response;
	if (!std::filesystem::exists(request.getFilePath()))
	{
		response.setStatus(StatusCode::NOT_FOUND);
		// response.setBody(error.getErrorPage(404));
		// response.
	}

	// std::ifstream		file(request.getFilePath());
	std::ifstream		file("html_pages/index.html");
	if (!file.is_open())
	{
		std::cerr << "Could not open " << std::endl;
	}
	std::stringstream	fileBuffer;
	std::string			fileContent;

	fileBuffer << file.rdbuf();
	fileContent = fileBuffer.str();
	file.close();
	response.setStatus(StatusCode::OK);
	response.setBody(fileContent);
	// response.setMimeType(getFileExtension(request.getFilePath()));
	response.setMimeType(getFileExtension("html_pages/index.html"));
	return(response);
}
HttpResponse		RequestHandler::handlePostRequest(const HttpRequest& request)
{
	(void)request;
	HttpResponse response;
	return(response);
}

HttpResponse		RequestHandler::handleDeleteRequest(const HttpRequest& request)
{
	(void)request;
	HttpResponse response;
	return(response);
}

std::string		getFileExtension(const std::string& filepath)
{
	return (std::filesystem::path(filepath).extension().string());
}
