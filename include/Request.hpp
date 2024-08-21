/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:04 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 07:55:44 by mleibeng         ###   ########.fr       */
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

enum class Method { GET, POST, DELETE };
class Request
{
	private:
	Method method;
	std::string path_name;
	std::string query_name;
	std::string body_name;
	std::unordered_map<std::string,std::string> header_names;
	public:

	Method			getMethod();
	std::string&	getPath();
	std::string&	getQuery();
	std::string&	getBody();
	std::unordered_map<std::string, std::string>& getHeaders();

	void parse(); //dont know what to return yet (definitely not void)
				  //maybe boolean if successfull or not?
};

#endif