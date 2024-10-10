#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

RequestHandler::RequestHandler(const RequestHandler &other)
{
	// std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler &other)
{
	if (this != &other)
	{
		// std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

RequestHandler::~RequestHandler()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}
