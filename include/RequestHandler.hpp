/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/01 00:33:39 by mleibeng         ###   ########.fr       */
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
#include <sys/wait.h>
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "CGICreator.hpp"
#include "Config.hpp"


struct ParsedPath
{
	std::string phys_path;
	std::string query;
};

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

		void setEnviron(const HttpRequest &request, const std::string& script_path, const CGIHandler& handler);
		void addEnvStuff(const std::string &name, const std::string& value);

		public:
		CGIEnv(const HttpRequest& request, const std::string& script_path, const CGIHandler& handler);
		~CGIEnv();

		char **getEnv();
	};

	struct PipeDescriptors
	{
		int in_pipe[2] = {-1, -1};
		int out_pipe[2] = {-1, -1};

		void closeAll();
		void closeParentPipes();
		void closeChildPipes();
	};

	bool setupPipes(PipeDescriptors &pipes, Client& client);
	void handleCGIChild(const PipeDescriptors &pipes, const std::string& script_path, HttpRequest& request, const CGIHandler& handler);
	void handleCGIParent(const PipeDescriptors &pipes, Client& client, const HttpRequest& request);
	std::string readCGIOutput(int pipe_fd);
	void writeCGIOutput(int pipe_fd, const std::string& body);
	void buildCGIResponse(const std::string& out, HttpResponse& response);

	public:
		RequestHandler() = delete;
		RequestHandler(const Config& config);
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void	handleRequest(Client& client);
		void	handleGetRequest(Client& client, const RouteConf& route_conf, const ParsedPath& parsed);
		void	handlePostRequest(Client& client, const RouteConf& route_conf, const ParsedPath& parsed);
		void	handleDeleteRequest(Client& client, const RouteConf& route_conf, const ParsedPath& parsed);

		const ServerConf *findServerConf(const HttpRequest &request);
		const RouteConf *findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		ParsedPath parsePath(const RouteConf& route_conf, const HttpRequest& request);
		bool isMethodAllowed(const RouteConf &route_conf, const std::string& method);

		void loadErrorPages();
		void serveErrorPage(Client& client, int error_code);

		void sendDirListing(Client &client, const std::string& dir_path);
		void sendFile(Client& client, const std::string& file_path);
		void handleCGI(Client& client, const std::string& cgi_path, const std::string& query);
		void handleFileUpload(int client_fd, const std::string& upload_dir);
};
std::string		getFileExtension(const std::string& filepath);

#endif
