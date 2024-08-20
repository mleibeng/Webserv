/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:56:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 07:08:46 by mleibeng         ###   ########.fr       */
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
	std::string pathToString;

	public:
	HandleCGI();
	void executeRequest(Request& request);
};

#endif