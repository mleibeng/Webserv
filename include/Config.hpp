/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:42:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:24:33 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Handle parsing of config file
Handles: configuration of hosts, ports and route structure on server-side.
*/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <optional>

struct EventsConf
{
	int worker_connects;
};

struct HttpConf
{
	std::unordered_map<std::string, std::string> directives;
	std::vector<ServerConf> servers;
};

struct RouteConf
{
	std::string path;
	std::vector<std::string> methods; // Define a list of accepted HTTPS methods for the route
	std::optional<std::string> redirect; // Define a HTTP redirection
	std::string root; //Define a directory or a file from where the file should be searched
	bool dir_listing_active; // turn on or off directory listing
	std::string default_file; // set default file ot answer if the request is a directory
	std::string cgi_extension; //execute CGI based on certain file extension ex: .php
	std::string upload_dir; // make the route able to accept uploaded file and configure where they should be saved.
	std::unordered_map<std::string, std::string> directives;
};

struct ServerConf
{
	std::string hostname; //host for each 'server'
	int port; // port for each 'server'
	std::vector<std::string> server_names; //'setup the server_names or not'
	std::string default_error_pages; //setup default error pages
	size_t max_body_size_client; //limit client body size
	std::unordered_map<std::string, RouteConf> routes; // setup routes with one or multiple of the following -> refer to RouteConf
	std::string listen;
	std::string index;
	int timeout;
	int max_connects;
	//probably also needs global settings for timeouts and max connections that server should handle etc..
};

class Config
{
	private:
	std::string conf_file;
	void parseEventBlock(std::string& eventBlock);
	void parseHttpBlock(std::string& httpBlock, std::regex& server_pattern, std::regex& route_pattern, std::regex& directives);

	public:
	Config(const std::string& conf_file);
	ServerConf parse();

	EventsConf events;
	HttpConf http;
};

#endif