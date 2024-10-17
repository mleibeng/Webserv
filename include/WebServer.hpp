/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/17 23:48:40 by mleibeng         ###   ########.fr       */
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
#include "Config.hpp"
#include "Loop.hpp"
#include "RequestHandler.hpp"


class Client;
class Loop;
class WebServer
{
	private:
	Config _config;
	std::unordered_map<std::string, std::vector<int>> _server_listeners;
	std::unordered_map<int, std::string> _error_pages;
	Loop _event_loop;
	std::atomic<bool> _running;
	std::unique_ptr<RequestHandler> request_handler;

	void setupListeners();
	void runLoop();
	void loadErrorPages();
	void acceptConnections(int listener_fd);
	int	 createNonBlockingSocket();
	void handleClientRequest(int client_fd);

	// Request/response handling part? could be moved out into other classes
	//---------------------------------------------------------------------------------------------

	public:
	WebServer() = default;
	explicit WebServer(const std::string& conf_file);
	~WebServer();

	void initialize();
	void start();
	void stop();
};

#endif