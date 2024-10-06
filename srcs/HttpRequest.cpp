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
