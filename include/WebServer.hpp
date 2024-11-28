/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/28 03:12:28 by mleibeng         ###   ########.fr       */
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

		void initialize(size_t pool_size);
		RequestHandler& getNextHandler();
		void start();
		void stop();

	private:
		struct ClientInfo
		{
			std::unique_ptr<Client> client;
			std::time_t last_active;

			ClientInfo(int fd, const Config& config);
		};

		std::map<int, ClientInfo> active_clients;

		Config config;
		std::unordered_map<std::string, std::vector<int>> server_listeners;
		Loop event_loop;
		std::atomic<bool> running;
		// std::unique_ptr<RequestHandler> request_handler;
		std::vector<std::unique_ptr<RequestHandler>> handler_pool;
		std::atomic<size_t> current_handler{0};

		void setupListeners();
		void runLoop();
		void acceptConnections(int listener_fd);
		int	 createNonBlockingSocket();
		void cleanInactiveClients();
		void handleClientRequest(int client_fd);
		void redirectTraffic(Client& client);
		bool portInUse(int port);
};

#endif
