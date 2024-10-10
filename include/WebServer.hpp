/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:14 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/09 23:08:30 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Main Class for the web server
Handles: Overall server cycle, including start stop, configurations and sockets
*/

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <future>
#include "Config.hpp"

class Loop;
class WebServer
{
	private:
	Config config;
	// std::unique_ptr<Loop> loop;
	std::vector<int> fd_listeners;
	std::future<void> server_threading;

	public:
	WebServer() = default;
	explicit WebServer(std::string &conf_file);
	~WebServer() = default;

	void start(); //maybe return status for logging
	void stop();

};

#endif