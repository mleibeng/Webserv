/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:04 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/18 05:02:21 by mleibeng         ###   ########.fr       */
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

#ifndef REQUEST_HPP
#define REQUEST_HPP

#endif