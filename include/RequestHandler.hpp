/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/17 23:48:13 by mleibeng         ###   ########.fr       */
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
#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Client.hpp"

class Error;
class RequestHandler
{
	public:
		RequestHandler();
		RequestHandler(const std::unordered_map<int, std::string>& _error_pages, const Config &config);
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		HttpResponse	handleRequest(const HttpRequest& request, const Client& client);
		HttpResponse	handleGetRequest(const HttpRequest& request, const Client& client);
		HttpResponse	handlePostRequest(const HttpRequest& request, const Client& client);
		HttpResponse	handleDeleteRequest(const HttpRequest& request, const Client& client);

		void loadErrorPage();
		std::string serveErrorPage(int error_code);
		void handleCGI(Client& client, const std::string& cgi_path, const std::string& query);
		void handleFileUpload(Client &client, const std::string& upload_dir);
		std::string		getFileExtension(const std::string& filepath);

	private:
	const std::unordered_map<int, std::string> &_error_pages;
	const Config& config;
};


#endif // REQUESTHANDLER_H
