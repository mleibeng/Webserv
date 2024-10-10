/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:56:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:14:36 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Execution of CGI scripts
Handles: dynamic content and script execution/ output return
*/

#ifndef HANDLECGI_HPP
#define HANDLECGI_HPP

#include <string>
#include "Request.hpp"

class HandleCGI
{
	private:
	std::string path_to_string;

	public:
	explicit HandleCGI(const std::string& path_to_string);
	std::string executeRequest(const Request& request) const;
};

#endif