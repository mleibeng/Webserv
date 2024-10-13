/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/13 18:55:24 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "EventLoop.hpp"
#include <iostream>
#include <string>

#define DEFAULT	"\033[0m"
#define RED		"\033[31m"
#define PORT	8080

int main(int argc, char** argv) {
	Config config;

	// config part
	if (argc == 1) {
		config.parse(std::string("config/NGINX1.conf"));
	}
	else if (argc == 2) {
		config.parse(argv[1]);
	}
	else {
		std::cerr << RED << "Usage ./webserv <configfile>" << DEFAULT << std::endl;
	}

	// epoll part
	EventLoop event_loop(PORT);
	event_loop.start();

	return 0;
}
