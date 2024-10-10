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
			throw std::exception(); //implement custom exception later
	}
}

std::string		RequestHandler::handleGetRequest(const HttpRequest& request)
{
	auto	iter = _getRequestHandlers.find(request.getUri());
	if (iter != _getRequestHandlers.end())
		return (iter->second(request));
	else
	{
		//handle error maybe try catch
	}
}

void			RequestHandler::registerGetHandler(const std::string& route, std::function<std::string(const HttpRequest&)> callback)
{
	_getRequestHandlers[route] = callback;
}

