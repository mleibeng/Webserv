/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/24 02:28:19 by mleibeng         ###   ########.fr       */
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
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Config.hpp"

class RequestHandler
{
	public:
		RequestHandler() = delete;
		RequestHandler(const Config& config);
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void			handleRequest(Client& client);
		void			handleGetRequest(Client& client, const RouteConf& route_conf);
		void			handlePostRequest(Client& client, const RouteConf& route_conf);
		void			handleDeleteRequest(Client& client, const RouteConf& route_conf);

		const ServerConf *findServerConf(const HttpRequest &request);
		const RouteConf *findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		bool isMethodAllowed(const RouteConf &route_conf, const std::string& method);

		void loadErrorPages();
		void serveErrorPage(int client_fd, int error_code);

		void sendDirListing(Client &client, const std::string& dir_path);
		void sendFile(Client& client, const std::string& file_path);
		void handleCGI(int client_fd, const std::string& cgi_path, const std::string& query);
		void handleFileUpload(int client_fd, const std::string& upload_dir);

	private:
		const Config& _config;
		std::unordered_map<int, std::string> _error_pages;
};
std::string		getFileExtension(const std::string& filepath);

#endif // REQUESTHANDLER_H
