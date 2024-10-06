#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(const HttpResponse &other);
		HttpResponse& operator=(const HttpResponse &other);
		~HttpResponse();

	private:

};

#endif // HTTPRESPONSE_H
