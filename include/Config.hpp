/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:42:52 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/15 22:45:12 by mleibeng         ###   ########.fr       */
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
#include <stdexcept>
#include <unordered_map>
#include <optional>

// we could also implement max_client_header_size for buffering the headersize. NGINX does this as well!

struct GlobalConf
{
	int _g_max_header_size = 1024; // limit client header size
	int _g_timeout = 60;  // set timeout for closing connections
	int _g_max_connects = 1000; // set maximum number of connections (to prevent DDoS)
	size_t _g_max_body_size = 1024 * 1024; //limit client body size
};

struct RouteConf
{
	std::vector<std::string> _methods; // Define a list of accepted HTTPS methods for the route
	std::optional<int> _port;
	std::optional<std::string> _redirect; // Define a HTTP redirection
	std::string _root; //Define a directory or a file from where the file should be searched
	bool _dir_listing_active = false; // turn on or off directory listing
	std::string _default_file; // set default file to answer if the request is a directory
	std::string _cgi_extension; //execute CGI based on certain file extension ex: .php
	std::string _upload_dir; // make the route able to accept uploaded file and configure where they should be saved.
	std::optional<int> _max_header_size; // limit client header size
	std::optional<size_t> _max_body_size; //limit client body size
	std::optional<int> _timeout; // set timeout for closing connections
	std::optional<int> _max_connects; // set maximum number of connections (to prevent DDoS)
	//could also implement body buffer size and header buffer size and amount for bigger bodies to control where they split;
};
struct ServerConf
{
	std::string _hostname; //host for each 'server'
	int _port = 80; // port for each 'server'
	std::vector<std::string> _server_names; //'setup the server_names or not'
	std::string _default_error_pages; //setup default error pages
	std::unordered_map<std::string, RouteConf> _routes; // setup routes with one or multiple of the following -> refer to RouteConf
};

class Config
{
	private:
	GlobalConf _globals;
	std::vector<ServerConf> _servers;

	public:
	Config() = default;
	static Config parse(const std::string& conf_file);
	const std::vector<ServerConf>& getServerConfs() const;
	static void parseServerBlock(ServerConf& conf, const std::string& key, const std::vector<std::string>& value);
	static void parseRouteBlock(RouteConf& conf, const std::string& key, const std::vector<std::string>& value);
	static std::string trim(const std::string &s);
	static void parseGlobalBlock(GlobalConf& conf, const std::string& key, const std::vector<std::string>& value);
	void print() const;
};

#endif