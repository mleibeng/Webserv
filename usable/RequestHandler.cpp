/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/14 15:49:39 by fwahl            ###   ########.fr       */
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
			return (handleGetRequest(request));
		//case Method::POST:	<--implement later
		//case Method::DELETE:	<--implement later
		default:
		{
			HttpResponse response;
			response.setStatus(StatusCode::NOT_IMPLEMENTED);
			response.setHeader("Content-Type", "text/plain");
			response.setBody("501 Not Implemented");
			return (response.buildResponse());
		}
	}
}

std::string		RequestHandler::handleGetRequest(const HttpRequest& request)
{
	//move this try / catch to higher lvl later
	try
	{
		auto	iter = _getRequestHandlers.find(request.getUri());
		if (iter != _getRequestHandlers.end())
		{
			std::string	content = iter->second(request);

			HttpResponse	response;
			response.setStatus(StatusCode::OK);
			response.setHeader("Content-Type", "text/html");
			response.setBody(content);
			return (response.buildResponse());
		}
		else
		{
			HttpResponse	response;
			response.setStatus(StatusCode::NOT_FOUND);
			response.setHeader("Content-Type", "text/plain");
			response.setBody("404 Not Found");
			return (response.buildResponse());
		}
	}
	catch (const std::exception& e)
	{
		HttpResponse response;
		response.setStatus(StatusCode::INTERNAL_SERV_ERR);
		response.setHeader("Content-Type", "text/plain");
		response.setBody("500 Internal Server Error: " + std::string(e.what()));
		return (response.buildResponse());
	}
}

std::string		RequestHandler::handlePostRequest(const HttpRequest& request)
{
	//move this try / catch to higher lvl later
	try
	{
		auto	iter = _postRequestHandlers.find(request.getUri());
		if (iter != _postRequestHandlers.end())
		{
			std::string	content = iter->second(request);

			HttpResponse	response;
			response.setStatus(StatusCode::OK);
			response.setHeader("Content-Type", "text/html");
			response.setBody(content);
			return (response.buildResponse());
		}
		else
		{
			HttpResponse	response;
			response.setStatus(StatusCode::NOT_FOUND);
			response.setHeader("Content-Type", "text/html");
			response.setBody("404 Not Found");
			return (response.buildResponse());
		}
	}
	catch(const std::exception& e)
	{
		HttpResponse	response;
		response.setStatus(StatusCode::INTERNAL_SERV_ERR);
		response.setHeader("Content-Type", "text/html");
		response.setBody("500 Internal Server Error: " + std::string(e.what()));
		return (response.buildResponse());
	}
}

void			RequestHandler::registerGetHandler(const std::string& route, std::function<std::string(const HttpRequest&)> callback)
{
	//move this try / catch to higher lvl later
	try
	{
		if (route.empty())
			throw (GetHandlerException("Route missing"));
		if (callback == nullptr)
			throw (GetHandlerException("Callback can't be null"));
		// not sure if this is necessary
		// if (_getRequestHandlers.find(route) != _getRequestHandlers.end())
		// 	throw (GetHandlerException("Route already registered"));
		_getRequestHandlers[route] = callback;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void			RequestHandler::registerPostHandler(const std::string& route,  std::function<std::string(const HttpRequest&)> callback)
{
	//move this try / catch to higher lvl later
	try
	{
		if (route.empty())
			throw (PostHandlerException("Route missing"));
		if (callback == nullptr)
			throw (PostHandlerException("Callback can't be null"));
		// not sure if this is necessary
		// if (_postRequestHandlers.find(route) != _postRequestHandlers.end())
		// 	throw (PostHandlerException("Route already registered"));
		_postRequestHandlers[route] = callback;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

RequestHandler::MethodHandlerException::MethodHandlerException(const std::string& errormsg) : _customErrorMsg(errormsg)
{
}

RequestHandler::GetHandlerException::GetHandlerException(const std::string& errormsg) : MethodHandlerException("GET Handler Exception: " + errormsg)
{
}

RequestHandler::PostHandlerException::PostHandlerException(const std::string& errormsg) : MethodHandlerException("POST Handler Exception: " + errormsg)
{
}

const char*	RequestHandler::MethodHandlerException::what() const noexcept
{
	static std::string	errorMsg = "RequestHandler: " + _customErrorMsg;
	return (errorMsg.c_str());
}
