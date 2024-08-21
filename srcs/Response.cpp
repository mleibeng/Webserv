/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:35 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 08:00:31 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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


#include "Response.hpp"

void Response::setStatus(StatusCode status)
{
	// change Status to input
	// and make check if it is even acceptable status as well?
}

void Response::setHeader(std::unordered_map<std::string,std::string>& header_set)
{
	// input the response header for client to receive
	// including all the processed data meant for the header.
}

void Response::setBody(std::string& body_set)
{
	// input the response body for client to receive
	// including all the processed data meant for the body.
}

std::string Response::toString()
{
	// process information like integers etc..
	// into string format to send as response.
}