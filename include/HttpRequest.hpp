#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>

class HttpRequest
{
	public:
		HttpRequest();
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		~HttpRequest();

	private:

};

#endif // HTTPREQUEST_H
