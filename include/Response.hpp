/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 09:51:02 by mleibeng         ###   ########.fr       */
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

enum class StatusCode {OK = 200, BAD_REQUEST = 400, NOT_FOUND = 404,};
//needs more like redirects some 300s and 500s as well etc.. for later

class Response
{
	private:
	StatusCode status = StatusCode::OK;
	std::unordered_map<std::string,std::string> header_set;
	std::string body_set;

	public:
	void setStatus(StatusCode status);
	void setHeader(std::unordered_map<std::string,std::string> header_set);
	void setBody(std::string& body_set);

	std::string toString();
};

#endif