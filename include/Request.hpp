/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:04 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 07:09:32 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: HTTP Request handling
Handles: Parsing, storage, handling of incoming HTTP requests and methods (GET,POST,DELETE)
*/

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>

enum class Method { GET, POST, DELETE};
class Request
{
	private:
	Method method;
	public:

	Method getMethod();
	// getPath(); probably string
	// getQuery(); probably string
	// getBody(); probably string
	// getHeaders(); probably unordered_map of string/string

	void parse(); //dont know what to return yet (definitely not void)
};

#endif