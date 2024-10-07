#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <string>
#include <map>



class HttpMessage
{
	public:
		HttpMessage();
		HttpMessage(const HttpMessage &other);
		HttpMessage& operator=(const HttpMessage &other);
		virtual ~HttpMessage();

		//setters
		void	setHttpVersion(const std::string& vers);
		void	setHeader(const std::string& key, const std::string& val);
		void	setBody(const std::string& body);



		//getters
		std::string		getHttpVersion() const;
		std::string		getHeader(const std::string& key) const;
		std::string		getBody() const;


	private:


	protected:
		std::string							_httpVersion;
		std::map<std::string, std::string>	_header;
		std::string							_body;


};

#endif // HTTPMESSAGE_H
