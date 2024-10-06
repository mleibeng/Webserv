#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

HttpMessage::HttpMessage(const HttpMessage &other)
{
	// std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

HttpMessage& HttpMessage::operator=(const HttpMessage &other)
{
	if (this != &other)
	{
		// std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

HttpMessage::~HttpMessage()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}
