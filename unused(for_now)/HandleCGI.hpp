/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:56:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/13 14:10:14 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Execution of CGI scripts
Handles: dynamic content and script execution/ output return
*/

#ifndef HANDLECGI_HPP
#define HANDLECGI_HPP

#include <string>

class HandleCGI
{
	private:
	std::string path_to_string;

	public:
	explicit HandleCGI(const std::string& path_to_string);
	std::string executeRequest(const Request& request) const;
};

#endif
