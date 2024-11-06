/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/06 14:20:19 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include "Config.hpp"
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "CGICreator.hpp"

class RequestHandler
{
	private:
		const Config& _config;
		std::unordered_map<int, std::string> _error_pages;

	class CGIEnv
	{
		private:

		static constexpr size_t BUFFER_SIZE = 4096;
		std::vector<char*> env_array;
		std::vector<std::unique_ptr<char[]>> env_bufs;

		void setEnviron(const HttpRequest &request, const std::string& cgi_path, const CGIHandler& handler);
		void addEnvStuff(const std::string &name, const std::string& value);

		public:
		CGIEnv(const HttpRequest& request, const std::string& cgi_path, const CGIHandler& handler);
		~CGIEnv();

		char **getEnv();
		void debugPrintEnv() const;
	};

	struct PipeDescriptors
	{
		int in_pipe[2] = {-1, -1};
		int out_pipe[2] = {-1, -1};

		void closeAll();
		void closeParentPipes();
		void closeChildPipes();
		void checkClose(int& fd);
	};

		bool setupPipes(PipeDescriptors &pipes, Client& client);
		void handleCGIChild(PipeDescriptors &pipes, const std::string& cgi_path, const HttpRequest& request, const CGIHandler& handler);
		void handleCGIParent(PipeDescriptors &pipes, Client& client, const HttpRequest& request);
		std::string readCGIOutput(int pipe_fd);
		void writeCGIInput(int pipe_fd, const std::string& body);
		void buildCGIResponse(const std::string& out, HttpResponse& response);
		void handleRedirect(const RouteConf& route_conf, Client& client);
		std::string buildRedirWQuery(const RouteConf& route_conf, const HttpRequest& request);

	public:
		RequestHandler() = delete;
		RequestHandler(const Config& config);
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void	handleRequest(Client& client);
		void	handleGetRequest(Client& client, const RouteConf& route_conf, const std::string& parsed);
		void	handleDeleteRequest(Client& client, const RouteConf& route_conf, const std::string& parsed);

		const ServerConf *findServerConf(const HttpRequest &request);
		const RouteConf *findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		std::string parsePath(const RouteConf& route_conf, const HttpRequest& request);
		bool isMethodAllowed(const RouteConf &route_conf, const std::string& method);

		void loadErrorPages();
		void serveErrorPage(Client& client, int error_code);

		void sendDirListing(Client &client, const std::string& dir_path);
		void sendFile(Client& client, const std::string& file_path);
		void handleCGI(Client& client, const std::string& cgi_path);

		void handlePostRequest(Client& client, const RouteConf& route_conf, const std::string& parsed);
		void handleFileUpload(Client& client, const std::string& content_type, const std::string& body);
		std::string extractBoundary(const std::string& content_type);
		std::string extractFile(const std::string& body, const std::string& boundary);
		std::string extractFileData(const std::string& file);
		std::string extractFilename(const std::string& file);
		void handleFormSubmission(Client& client, const std::string& body);
};

std::string		getFileExtension(const std::string& filepath);

#endif
