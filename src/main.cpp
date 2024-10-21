/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/21 21:12:00 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>
#include <string>
#include "WebServer.hpp"

#define DEFAULT	"\033[0m"
#define RED		"\033[31m"
#define PORT	8080

int main(int argc, char** argv) {
	std::string config_file;

	if (argc != 2)
		config_file = "Configs/NGINX1.conf";
	else
		config_file = argv[1];
	try
	{
		WebServer MainServ(config_file);
		MainServ.initialize();
		MainServ.start();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
