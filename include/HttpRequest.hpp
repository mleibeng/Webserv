/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:33 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/08 02:26:09 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <string>
#include "AHttpMessage.hpp"

// The following example illustrates a typical message exchange for a
//    GET request (Section 4.3.1 of [RFC7231]) on the URI in ABNF Format
//    "http://www.example.com/hello.txt":

//    Client request:

//      GET /hello.txt HTTP/1.1
//      User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
//      Host: www.example.com
//      Accept-Language: en, mi


//    Server response:

//      HTTP/1.1 200 OK
//      Date: Mon, 27 Jul 2009 12:28:53 GMT
//      Server: Apache
//      Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
//      ETag: "34aa387-d-1568eb00"
//      Accept-Ranges: bytes
//      Content-Length: 51
//      Vary: Accept-Encoding
//      Content-Type: text/plain

//      Hello World! My payload includes a trailing CRLF.

enum class Method { DEFAULT, GET, POST, DELETE };


class HttpRequest : public AHttpMessage
{
	public:
		HttpRequest();
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		~HttpRequest();

		//parse
		bool	parse(const std::string& rawmsg) override;


		class InvalidMethodException : public std::exception
		{
			std::string	_invalidMethod;
			public:
				explicit InvalidMethodException(const std::string& method);
				const char*	what() const noexcept override;
		};

	private:
		std::string	_uri; //Uniform Resource Identifier
		Method		_method;

		void		setMethod(Method method);
		void		setUri(const std::string& uri);

		//getters
		Method		getMethod() const;
		std::string	getUri() const;

		//utils
		Method		strToMethod(const std::string& methodStr);
		std::string	methodToStr() const;
};

#endif // HTTPREQUEST_H
