/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:10 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:47:37 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Utility functions for different steps
Handles: Probably a good file to put templates and security features into!
Prevent XSS attacks:
-> only allow loading of scripts from certain directories
-> add security related HTTP Headers.
Prevent CSRF attacks: (very difficult)
-> implement token validation on server-side
-> Set cookie patterns and SameSite Cookies
-> should be modified in request and response classes!
Prevent DOS attacks:
-> Rate Limiting means limiting the number of requests a single client can make in a time period
-> Set limit for maximum size of request body lower
-> ensure file uploads have resource limits or are managed properly.
-> Connection Throttling to limit the number of simultaneous connections out of one IP Address
*/

#ifndef UTILS_HPP
#define UTILS_HPP

// std::string escapeInput() would be good to prevent XSS attacks
// std::string encodeOutput() would be good to prevent XSS attacks
// maybe timer functions here as well>
// maybe also checking functions that log the size of requests,
// -> resource requirements of uploads
// -> and simultaneous connections a single IP Address has.

#endif