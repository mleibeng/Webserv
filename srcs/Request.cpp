/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 07:55:11 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


#include "Request.hpp"

Method Request::getMethod()
{
	return method;
}

std::string& Request::getPath()
{
	return path_name;
}

std::string& Request::getBody()
{
	return body_name;
}

std::string& Request::getQuery()
{
	return query_name;
}

std::unordered_map<std::string,std::string>& Request::getHeaders()
{
	return header_names;
}

void Request::parse()
{
	//read in the request and save the necessary information in the variables mentioned!!
}
