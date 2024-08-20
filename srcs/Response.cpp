/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:35 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 03:05:20 by mleibeng         ###   ########.fr       */
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