/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:57 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/30 00:32:37 by mleibeng         ###   ########.fr       */
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
#include "Config.hpp"

class RequestHandler
{
	struct PipeDescriptors
	{
		int in_pipe[2];
		int out_pipe[2];

		void closeAll();
	};

	class CGIEnvironment
	{
		private:
		static const size_t BUFFER_SIZE = 4096;
		char req_method[BUFFER_SIZE];
		char query_string[BUFFER_SIZE];
		char content_length[BUFFER_SIZE];
		char script_filename[BUFFER_SIZE];
		char redirect_status[BUFFER_SIZE];
		char content_type[BUFFER_SIZE];
		std::vector<char*> env_array;

		public:
		void setupEnvironment(const std::string& query, const std::string& cgi_path);
		char** getEnvpArray();

		~CGIEnvironment();
	};

	public:
		RequestHandler() = delete;
		RequestHandler(const Config& config);
		RequestHandler(const RequestHandler &other) = delete; //might implement this later if needed
		RequestHandler& operator=(const RequestHandler &other) = delete; //might implement this later if needed;
		~RequestHandler();

		void	handleRequest(Client& client);
		void	handleGetRequest(Client& client, const RouteConf& route_conf);
		void	handlePostRequest(Client& client, const RouteConf& route_conf);
		void	handleDeleteRequest(Client& client, const RouteConf& route_conf);

		const ServerConf *findServerConf(const HttpRequest &request);
		const RouteConf *findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		bool isMethodAllowed(const RouteConf &route_conf, const std::string& method);

		void loadErrorPages();
		void serveErrorPage(Client& client, int error_code);

		void sendDirListing(Client &client, const std::string& dir_path);
		void sendFile(Client& client, const std::string& file_path);
		void handleCGI(Client& client, const std::string& cgi_path, const std::string& query);
		void handleFileUpload(int client_fd, const std::string& upload_dir);


	private:
		const Config& _config;
		std::unordered_map<int, std::string> _error_pages;

	bool setupPipes(PipeDescriptors &pipes, Client& client);
	void handleChildProc(const PipeDescriptors& pipes, const std::string& cgi_path, CGIEnvironment &env);
	std::string readCGIOutput(int pipe_fd);
	void parseCGIOutput(const std::string& cgi_output, HttpResponse& response);
};
std::string		getFileExtension(const std::string& filepath);

#endif // REQUESTHANDLER_H
