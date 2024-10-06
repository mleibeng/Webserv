#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>

class HttpMessage
{
	public:
		HttpMessage();
		HttpMessage(const HttpMessage &other);
		HttpMessage& operator=(const HttpMessage &other);
		~HttpMessage();

	private:

};

#endif // HTTPMESSAGE_H
