/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:37 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/10 03:53:56 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "HeaderIncludes.hpp"
#include "AHttpMessage.hpp"

/*
Purpose: HTTP Response handling
Handles: Parsing, storage, handling of outgoing HTTP responses and methods (status codes, headers, body),
-> 		 as well as conversion to string for sending.
*/

// 	  Most HTTP communication consists of a retrieval request (GET) for a
//    representation of some resource identified by a URI.  In the simplest
//    case, this might be accomplished via a single bidirectional
//    connection (===) between the user agent (UA) and the origin
//    server (O).

//             request   >
//        UA ======================================= O
//                                    <   response

//  A "gateway" (a.k.a. "reverse proxy") is an intermediary that acts as
//    an origin server for the outbound connection but translates received
//    requests and forwards them inbound to another server or servers.
//    Gateways are often used to encapsulate legacy or untrusted
//    information services, to improve server performance through
//    "accelerator" caching, and to enable partitioning or load balancing
//    of HTTP services across multiple machines.

enum class StatusCode
{
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NOCONTENT = 204,
	MOVED_PERM = 301,
	MOVED_TEMP = 302,
	NOT_MODIFIED = 304,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METH_NOT_ALLOWED = 405,
	INTERNAL_SERV_ERR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAIL = 503,
	ENDLESS_LOOP = 508
};

class HttpResponse : public AHttpMessage
{
	public:
		HttpResponse();
		HttpResponse(const HttpResponse &other);
		HttpResponse& operator=(const HttpResponse &other);
		~HttpResponse();

		void setCookie(const std::string& key, const std::string& value,
									int maxAge = -1, const std::string &path = "/",
									bool secure = false, bool HttpOnly = false);
		void removeCookie(const std::string& key, const std::string& path = "/");
		std::string	buildResponse() const;

		bool	parse(const std::string& rawmsg) override;

		void		setStatus(int status);
		void		setStatus(StatusCode status);
		void		setMimeType(std::string extension);
		std::string	getMimeType(const std::string extension);
		int	getStatus() const;

	private:
		std::string	stostr() const;
		int			stoi() const;
		StatusCode	itos(int code) const;

		StatusCode	_status;
		std::string	_mimeType;
		std::vector<std::string> _response_cookies;
};

#endif // HTTPRESPONSE_H
