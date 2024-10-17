/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/17 19:17:56 by fwahl            ###   ########.fr       */
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
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class RequestHandler
{
	public:
		RequestHandler();
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void			registerGetHandler(const std::string& route, std::function<std::string(const HttpRequest&)> callback);
		void			registerPostHandler(const std::string& route,  std::function<std::string(const HttpRequest&)> callback);

		std::string		handleRequest(const HttpRequest& request);
		std::string		handleGetRequest(const HttpRequest& request);
		std::string		handlePostRequest(const HttpRequest& request);
		std::string		handleDeleteRequest(const HttpRequest& request);

		class MethodHandlerException : public std::exception
		{
			protected:
				std::string	_customErrorMsg;
			public:
				explicit MethodHandlerException(const std::string& errormsg);
				const char*	what() const noexcept override;
		};

		class GetHandlerException : public MethodHandlerException
		{
			public:
				explicit GetHandlerException(const std::string& errormsg);
		};

		class PostHandlerException : public MethodHandlerException
		{
			public:
				explicit PostHandlerException(const std::string& errormsg);
		};


	private:
		std::map<std::string, std::function<std::string(const HttpRequest&)>> _getRequestHandlers;
		std::map<std::string, std::function<std::string(const HttpRequest&)>> _postRequestHandlers;

};

#endif // REQUESTHANDLER_H
