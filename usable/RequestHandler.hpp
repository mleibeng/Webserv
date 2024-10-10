#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include "HttpRequest.hpp"

class RequestHandler
{
	public:
		RequestHandler();
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void			registerGetHandler(const std::string& route, std::function<std::string(const HttpRequest&)> callback);

		std::string		handleRequest(const HttpRequest& request);
		std::string		handleGetRequest(const HttpRequest& request);

	private:
		std::map<std::string, std::function<std::string(const HttpRequest&)>> _getRequestHandlers;

};

#endif // REQUESTHANDLER_H
