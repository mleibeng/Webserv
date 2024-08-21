/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:42:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 08:03:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Handle parsing of config file
Handles: configuration of hosts, ports and route structure on server-side.
*/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

struct RouteConf
{
	std::vector<std::string> methods;
	std::optional<std::string> redirect;
	std::string root;
	bool dir_listen_active;
	std::string default_file;
	std::string cgi_extension;
	std::string upload_dir;
};

struct ServerConf
{
	std::string hostname;
	int port;
	std::vector<std::string> server_names;
	std::string default_error_pages;
	size_t max_body_size_client;
	std::unordered_map<std::string, RouteConf> routes;
	//probably also needs global setting etc..
};

class Config
{
	private:
	std::string conf_file;

	public:
	Config(const std::string& conf_file);
	std::vector<ServerConf> parse();
};

#endif