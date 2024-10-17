/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:15 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/17 22:02:57 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <algorithm>
#include <iostream>

size_t parseSizeNotation(const std::string& sizeStr)
{
	std::unordered_map<char, size_t> multipliers =
	{
		{'K', 1024},
		{'M', 1024 * 1024},
		{'G', 1024 * 1024 * 1024}
	};
	size_t size = 0;
	size_t currentNumber = 0;

	for (char c : sizeStr)
	{
		if (std::isdigit(c))
			currentNumber = currentNumber * 10 + (c - '0');
		else if (std::isalpha(c))
		{
			char unit = std::toupper(c);
			if (multipliers.find(unit) == multipliers.end())
				throw std::runtime_error("Invalid size unit: " + std::string(1, c));
			size += currentNumber * multipliers[unit];
			currentNumber = 0;
		}
		else if (c != ' ' && c != '\t')
			throw std::runtime_error("Invalid character in size notation: " + std::string(1, c));
	}
	if (currentNumber > 0) {
		size += currentNumber;
	}
	return size;
}

std::string Config::trim(const std::string &s)
{
	auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return std::isspace(c); });
	auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return std::isspace(c); }).base();
	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

void Config::parseServerBlock(ServerConf& conf, const std::string& key, const std::vector<std::string>& values)
{
	if (key == "hostname")
		conf._hostname = values[0];
	else if (key == "port")
		conf._port = std::stoi(values[0]);
	else if (key == "server_name")
		conf._server_names = values;
	else if (key == "default_error_pages")
		conf._default_error_pages = values[0];
	else
		throw std::runtime_error("Unknown Server Configuration key");
}

void Config::parseRouteBlock(RouteConf& conf, const std::string& key, const std::vector<std::string>& values)
{
	if (key == "methods")
		conf._methods = values;
	else if (key == "redirect")
		conf._redirect = values[0];
	else if (key == "port")
		conf._port = std::stoi(values[0]);
	else if (key == "root")
		conf._root = values[0];
	else if (key == "dir_listing")
		conf._dir_listing_active = (values[0] == "on" || values[0] == "true" || values[0] == "1");
	else if (key == "default_file")
		conf._default_file = values[0];
	else if (key == "cgi_extension")
		conf._cgi_extension = values[0];
	else if (key == "upload_dir")
		conf._upload_dir = values[0];
	else if (key == "max_body_size")
		conf._max_body_size = parseSizeNotation(values[0]);
	else if (key == "max_header_size")
		conf._max_header_size = parseSizeNotation(values[0]);
	else if (key == "timeout")
		conf._timeout = std::stoi(values[0]);
	else if (key == "max_connects")
		conf._max_connects = std::stoi(values[0]);
	else
		throw std::runtime_error("Unknown Route Configuration key");
}

void Config::parseGlobalBlock(GlobalConf& conf, const std::string& key, const std::vector<std::string>& values)
{
	if (key == "timeout")
		conf._g_timeout = std::stoi(values[0]);
	else if (key == "max_connects")
		conf._g_max_connects = std::stoi(values[0]);
	else if (key == "max_body_size")
		conf._g_max_body_size = parseSizeNotation(values[0]);
	else if (key == "max_header_size")
		conf._g_max_header_size = parseSizeNotation(values[0]);
	else
		throw std::runtime_error("Unknown Global Configuration key");
}

Config Config::parse(const std::string& conf_file)
{
	Config config;
	std::ifstream file(conf_file);
	if (!file.is_open())
		throw std::runtime_error("Unable to open config file!");

	std::string line;
	ServerConf current_server;
	RouteConf current_route;
	std::string current_route_path;
	bool in_server_block = false;
	bool in_route_block = false;
	int line_number = 0;

	while (std::getline(file, line))
	{
		line_number++;
		line = trim(line);
		if (line.empty() || line[0] == '#')
			continue;
		if (line == "server {")
		{
			in_server_block = true;
			current_server = ServerConf();
		}
		else if (line.substr(0,6) == "route " && line.back() == '{')
		{
			if (!in_server_block)
				throw std::runtime_error("route block outside of server block at line: " + std::to_string(line_number));
			in_route_block = true;
			current_route = RouteConf();
			current_route_path = trim(line.substr(6, line.find('{') - 7));
			std::cout << current_route_path << std::endl;
		}
		else if (line == "}")
		{
			if (in_route_block)
			{
				in_route_block = false;
				current_server._routes[current_route_path] = current_route;
				current_route_path.clear();
			}
			else if (in_server_block)
			{
				in_server_block = false;
				config._servers.push_back(current_server);
			}
			else
				throw std::runtime_error("Unexpected closing brace at line: " + std::to_string(line_number));
		}
		else
		{
			std::istringstream iss(line);
			std::string key;
			std::vector<std::string> values;
			if (iss >> key)
			{
				std::string value;
				while (iss >> value)
					values.push_back(value);
				if (in_route_block)
					parseRouteBlock(current_route, key, values);
				else if (in_server_block)
					parseServerBlock(current_server, key, values);
				else
					parseGlobalBlock(config._globals, key, values);
			}
			else
				throw std::runtime_error("invalid config line at: "  + std::to_string(line_number));
		}
	}
	for (auto& server : config._servers)
	{
		for (auto& [path, route] : server._routes)
		{
			if (!route._max_header_size) route._max_header_size = config._globals._g_max_header_size;
			if (!route._max_body_size) route._max_body_size = config._globals._g_max_body_size;
			if (!route._timeout) route._timeout = config._globals._g_timeout;
			if (!route._max_connects) route._max_connects = config._globals._g_max_connects;
			if (!route._port) route._port = current_server._port;
		}
	}
	return config;
}

const std::vector<ServerConf>& Config::getServerConfs() const
{return _servers;}

void Config::print() const
{
	std::cout << "Global Configuration:\n";
	std::cout << "  Max Header Size: " << _globals._g_max_header_size << "\n";
	std::cout << "  Max Body Size: " << _globals._g_max_body_size << "\n";
	std::cout << "  Timeout: " << _globals._g_timeout << "\n";
	std::cout << "  Max Connects: " << _globals._g_max_connects << "\n\n";

	for (const auto& server : _servers)
	{
		std::cout << "Server Configuration:\n";
		std::cout << "  Hostname: " << server._hostname << "\n";
		std::cout << "  Port: " << server._port << "\n";
		std::cout << "  Server Names: ";
		for (const auto& name : server._server_names)
			std::cout << name << " ";
		std::cout << "\n";
		std::cout << "  Default Error Pages: " << server._default_error_pages << "\n";

		for (const auto& [path, route] : server._routes)
		{
			std::cout << "  Route: " << path << "\n";
			std::cout << "    Methods: ";
			for (const auto& method : route._methods)
				std::cout << method << " ";
			std::cout << "\n";
			if (route._redirect)
				std::cout << "    Redirect: " << *route._redirect << "\n";
			std::cout << "    Route Port: " << *route._port << "\n";
			std::cout << "    Root: " << route._root << "\n";
			std::cout << "    Directory Listing: " << (route._dir_listing_active ? "On" : "Off") << "\n";
			std::cout << "    Default File: " << route._default_file << "\n";
			std::cout << "    CGI Extension: " << route._cgi_extension << "\n";
			std::cout << "    Upload Directory: " << route._upload_dir << "\n";
			std::cout << "    Max Header Size: " << *route._max_header_size << "\n";
			std::cout << "    Max Body Size: " << *route._max_body_size << "\n";
			std::cout << "    Timeout: " << *route._timeout << "\n";
			std::cout << "    Max Connects: " << *route._max_connects << "\n";
		}
		std::cout << "\n";
	}
}
