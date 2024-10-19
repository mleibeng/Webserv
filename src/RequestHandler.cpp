/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/19 15:48:49 by mott             ###   ########.fr       */
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

std::string		RequestHandler::handleRequest(const HttpRequest& request)
{
	switch(request.getMethod())
	{
		case Method::GET:
			return (handleGetRequest(request).buildResponse());
		case Method::POST:
			return (handlePostRequest(request).buildResponse());
		case Method::DELETE:
			return (handleDeleteRequest(request).buildResponse());
		default:
		{
			HttpResponse response;
			response.setStatus(StatusCode::NOT_IMPLEMENTED);
			// response.setBody(error.getErrorPage(501));
			response.setHeader("Content-Type", "html/txt");
			return (response.buildResponse());
		}
	}
}

HttpResponse		RequestHandler::handleGetRequest(const HttpRequest& request)
{
	HttpResponse response;
	// if (!std::filesystem::exists(request.getFilePath()))
	// {
	// 	response.setStatus(StatusCode::NOT_FOUND);
	// 	// response.setBody(error.getErrorPage(404));
	// 	response.setHeader("Content-Type", "html/txt");
	// }

	// std::ifstream		file(request.getFilePath());
	// std::ifstream		file("html_pages/index.html");
	// std::cout << RED << request.getUri() << RESET << std::endl;
	std::ifstream		file("html_pages/" + request.getUri());
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
	// response.setMimeType(getFileExtension("html_pages/index.html"));
	response.setMimeType(getFileExtension("html_pages/" + request.getUri()));
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
