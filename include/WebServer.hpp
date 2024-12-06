/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/12/06 04:30:57 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Main Class for the web server
Handles: Overall server cycle, including start stop, configurations and sockets
*/

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "HeaderIncludes.hpp"
#include "Config.hpp"
#include "Loop.hpp"
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestHandler.hpp"

class Loop;
class Client;
class RequestHandler;

class WebServer
{
	public:
		WebServer() = default;
		explicit WebServer(const std::string& conf_file);
		~WebServer();

		void	initialize(size_t pool_size);
		void	start();
		void	stop();
		RequestHandler&	getNextHandler();

	private:
		struct ClientInfo
		{
			std::unique_ptr<Client> client;
			std::time_t last_active;

			ClientInfo(int fd, const Config& config);
		};

		std::map<int, ClientInfo> active_clients;

		Config												config;
		Loop												event_loop;
		std::atomic<bool>									running;
		std::atomic<size_t>									current_handler{0};
		std::vector<std::unique_ptr<RequestHandler>>		handler_pool;
		std::unordered_map<std::string, std::vector<int>>	server_listeners;

		void	setupListeners();
		void	runLoop();
		void	acceptConnections(int listener_fd);
		void	cleanInactiveClients();
		int		createNonBlockingSocket();
		bool	portInUse(int port);
		ssize_t	handleClientRequest(int client_fd);
		bool	isComplete(const std::string& request);
};

#endif
