/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/17 23:48:16 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

RequestHandler::RequestHandler(const std::unordered_map<int, std::string>& _error_pages, const Config &config)
: _error_pages(_error_pages), config(config) {}

RequestHandler::~RequestHandler()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

std::string RequestHandler::serveErrorPage(int error_code)
{

}

HttpResponse		RequestHandler::handleRequest(const HttpRequest& request, const Client& client)
{
	switch(request.getMethod())
	{
		case Method::GET:
			return (handleGetRequest(request, client));
		case Method::POST:
			return (handlePostRequest(request, client));
		case Method::DELETE:
			return (handleDeleteRequest(request, client));
		default:
		{
			HttpResponse response;
			response.setStatus(StatusCode::NOT_IMPLEMENTED);
			response.setBody(serveErrorPage(501));
			response.setHeader("Content-Type", "html/txt");
		}
	}
}

HttpResponse		RequestHandler::handleGetRequest(const HttpRequest& request, const Client& client)
{
	HttpResponse response;
	if (!std::filesystem::exists(request.getFilePath()))
	{
		response.setStatus(StatusCode::NOT_FOUND);
		response.setBody(serveErrorPage(404));
		response.setMimeType(".html");
	}

	std::ifstream		file(request.getFilePath());
	std::stringstream	fileContent;

	fileContent << file.rdbuf();
	response.setStatus(StatusCode::OK);
	response.setBody(fileContent.str());
	response.setMimeType(getFileExtension(request.getFilePath()));

}
HttpResponse		RequestHandler::handlePostRequest(const HttpRequest& request, const Client& client)
{

}

HttpResponse		RequestHandler::handleDeleteRequest(const HttpRequest& request, const Client& client)
{

}

std::string		getFileExtension(const std::string& filepath)
{
	return (std::filesystem::path(filepath).extension().string());
}
