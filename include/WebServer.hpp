/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/03 22:24:39 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Main Class for the web server
Handles: Overall server cycle, including start stop, configurations and sockets
*/

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <atomic>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <cstring>
#include <memory>
#include "Config.hpp"
#include "Loop.hpp"
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestHandler.hpp"

class Loop;
class Client;
class WebServer
{
	private:

	struct ClientInfo
	{
		std::unique_ptr<Client> client;
		std::time_t last_active;

		ClientInfo(int fd);
	};

	std::map<int, ClientInfo> active_clients;

	Config config;
	std::unordered_map<std::string, std::vector<int>> server_listeners;
	Loop event_loop;
	std::atomic<bool> running;
	std::unique_ptr<RequestHandler> request_handler;

	void setupListeners();
	void runLoop();
	void acceptConnections(int listener_fd);
	int	 createNonBlockingSocket();
	void handleClientRequest(int client_fd, RequestHandler& handler);

	void cleanInactiveClients();

	public:
	WebServer() = default;
	explicit WebServer(const std::string& conf_file);
	~WebServer();

	void initialize();
	void start();
	void stop();
};

#endif
