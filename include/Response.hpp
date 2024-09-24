/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:03:43 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: HTTP Response handling
Handles: Parsing, storage, handling of outgoing HTTP responses and methods (status codes, headers, body),
-> 		 as well as conversion to string for sending.
*/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <unordered_map>

enum class StatusCode
{	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NOCONTENT = 204,
	MOVED_PERM = 301,
	FOUND = 302,
	NOT_MODIFIED = 304,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	INTERNAL_SERV_ERR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAIL = 503
};

class Response
{
	private:
	StatusCode status = StatusCode::OK;
	std::unordered_map<std::string,std::string> header_set;
	std::string body_set;

	public:
	void setStatus(StatusCode status);
	void setHeader(std::unordered_map<std::string,std::string>& header_set);
	void setBody(std::string& body_set);

	std::string toString();
};

#endif