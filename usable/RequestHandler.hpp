#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <functional>
#include <map>
#include <string>

class RequestHandler
{
	public:
		RequestHandler();
		RequestHandler(const RequestHandler &other);
		RequestHandler& operator=(const RequestHandler &other);
		~RequestHandler();

	private:

};

#endif // REQUESTHANDLER_H
