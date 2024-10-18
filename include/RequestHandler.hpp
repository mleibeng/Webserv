/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/18 17:00:06 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <exception>
#include <filesystem>
#include <fstream>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class Error;
class RequestHandler
{
	public:
		RequestHandler();
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		HttpResponse	handleRequest(const HttpRequest& request);
		HttpResponse	handleGetRequest(const HttpRequest& request);
		HttpResponse	handlePostRequest(const HttpRequest& request);
		HttpResponse	handleDeleteRequest(const HttpRequest& request);

	private:

};
std::string		getFileExtension(const std::string& filepath);

#endif // REQUESTHANDLER_H
