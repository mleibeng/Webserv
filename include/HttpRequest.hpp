/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:33 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/07 02:43:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "HeaderIncludes.hpp"
#include "Config.hpp"
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

class HttpRequest : public AHttpMessage
{
	public:
		enum class State
		{
			R_HEADER,
			ROUTING,
			R_BODY,
			COMPLETE,
			ERROR
		};

		HttpRequest();
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		~HttpRequest();

		//parse
		bool	parse(const std::string& rawmsg) override;
		bool	parseHeaderChunk(const std::string& chunky);
		bool	parseBodyChunk(const std::string& chunky);

		//setters
		void	setMethod(const std::string& method);
		void	setQuery(const std::string& uri);
		void	initUpload(const RouteConf& route);

		//getters
		const std::string&	getMethod() const; //check if ok with &!
		const std::string&	getUri() const; // same
		const std::string&	getQuery() const;

		State getState() const { return _state; }
		void setState(State state) { _state = state; }
		const std::string& getUploadPath() const { return _upload_path; }
		bool Complete() const { return _state == State::COMPLETE; }

	private:
		State _state;
		std::string	_method;
		std::string	_uri; //Uniform Resource Identifier
		std::string _query;
		std::string _part_headers;
		std::string _upload_path;
		std::unique_ptr<std::ofstream> _upload_file;
		size_t _expected_content_len;
		size_t _byte_read;
		std::string _boundary;
};

#endif // HTTPREQUEST_H
