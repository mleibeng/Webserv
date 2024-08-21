/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:15 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:18:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

Config::Config(const std::string& conf_file) : conf_file(conf_file) {}

std::vector<ServerConf> Config::parse()
{
	std::vector<ServerConf> server_confs;
	std::ifstream file(conf_file);
	if(!file.is_open())
		throw std::runtime_error("Can't open conf file");
	std::string line;
	ServerConf current_serv;
	RouteConf current_route;
	bool in_serv = false;
	bool in_route = false;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string key, value;
		std::getline(ss, key, ':');
		std::getline(ss, value);
	}

	//clean up keys and values by trimming;

	//split up key search for settings: global, server, route, host, body, error_pages etc..
	//to fill out necessary data structs.
}
