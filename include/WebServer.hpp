/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/10 15:09:39 by marvinleibe      ###   ########.fr       */
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
#include <future>
#include <atomic>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "Config.hpp"

class Loop;
class WebServer
{
	private:
	Config config;
	std::unordered_map<std::string, std::vector<int>> server_listeners;
	std::future<void> server_thread;
	std::atomic<bool> running;

	public:
	WebServer() = default;
	explicit WebServer(const std::string& conf_file);
	~WebServer();

	void initialize();
	void start();
	void stop();

	private:
	void setupListeners();
	void acceptConnections();
	int createNonBlockingSocket();
};

#endif