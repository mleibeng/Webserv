/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/13 19:50:11 by mleibeng         ###   ########.fr       */
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

class Loop;
class WebServer
{
	private:
	Config config;
	std::unordered_map<std::string, std::vector<int>> server_listeners;
	std::unordered_map<int, std::string> error_pages;
	Loop event_loop;
	std::atomic<bool> running;

	void setupListeners();
	void runLoop();
	void loadErrorPages();
	void acceptConnections(int listener_fd);
	int createNonBlockingSocket();
	void handleClientRequest(int client_fd);
	void serveErrorPage(int client_fd, int error_code);
	void handleCGI(int client_fd, const std::string& cgi_path, const std::string& query);
	void handleFileUpload(int client_fd, const std::string& upload_dir);
	std::string getErrorPage(int error_code);
	void addToEventSystem( int listener_fd, int event);
	void removeFromEventSystem(int fd);

	public:
	WebServer() = default;
	explicit WebServer(const std::string& conf_file);
	~WebServer();

	void initialize();
	void start();
	void stop();
};

#endif