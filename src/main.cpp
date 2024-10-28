/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/28 00:57:30 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>
#include <string>
#include <csignal>
#include "WebServer.hpp"

#define DEFAULT	"\033[0m"
#define RED		"\033[31m"

static WebServer* g_server_ptr = nullptr;

void sighandler(int signum)
{
	if (signum == SIGINT && g_server_ptr)
		g_server_ptr->stop();
}

int main(int argc, char** argv) // now where to build a signal handler...
{
	std::string config_file;

	if (argc != 2)
		config_file = "Configs/NGINX1.conf";
	else
		config_file = argv[1];
	try
	{
		WebServer MainServ(config_file);
		g_server_ptr = &MainServ;
		std::signal(SIGINT, sighandler);
		MainServ.initialize();
		MainServ.start();
		g_server_ptr = nullptr;
	}
	catch (const std::runtime_error& e)
	{
		g_server_ptr = nullptr;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
