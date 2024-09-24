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

Config::Config(const std::string& conf_file) : conf_file(conf_file) {}


void Config::parseEventBlock(std::string& eventBlock)
{
	std::regex event_directives(R"((\w+)\s+([^;]+);)");
	auto iter = std::sregex_iterator(eventBlock.begin(), eventBlock.end(), event_directives);
        auto end = std::sregex_iterator();

        while (iter != end) {
            std::string key = (*iter)[1].str();
            std::string value = (*iter)[2].str();
            if (key == "worker_connections") {
                events.worker_connects = std::stoi(value);
            }
            ++iter;
        }
}

void Config::parseServerBlock(std::string& block, ServerConf &server, std::regex& route_pattern, std::regex& directives)
{
	auto route_iter = std::sregex_iterator(block.begin(), block.end(), route_pattern);
	auto directive_iter = std::sregex_iterator(block.begin(), block.end(), directives);
	auto end = std::sregex_iterator();

	while(directive_iter != end)
	{
		std::string key = (*directive_iter)[1].str();
		std::string value = (*directive_iter)[2].str();
		if (key == "listen")
		{
			server.listen = value;
			auto pos = value.find(':');
			if (pos != std::string::npos)
			{
				server.hostname = value.substr(0, pos);
				server.port = std::stoi(value.substr(pos + 1));
			}
			else
			{
				server.port = std::stoi(value);
			}
		}
		else if (key == "server_name") 
		{
			std::istringstream server_string(value);
			std::string name;
			while(server_string >> name)
			{
				server.server_names.push_back(name);
			}
		}
		else if (key == "index")
			server.index = value;
		else if (key == "error_page")
			server.default_error_pages = value;
		else if (key == "client_max_body_size")
			server.max_body_size_client = std::stoull(value);
		else if (key == "timeout")
			server.timeout = std::stoi(value);
		else if (key == "max_connections")
			server.max_connects = std::stoi(value);
		++directive_iter;
		// might need more here depending on the config
		// also maybe set default values here in case none are specified but it is necessary for utilisation !
	}

	while (route_iter != end)
	{
		//route parser here just like those before.
	}
}

void Config::parseHttpBlock(std::string& httpBlock, std::regex& server_pattern, std::regex& route_pattern, std::regex& directives)
{
	auto server_iter = std::sregex_iterator(httpBlock.begin(), httpBlock.end(), server_pattern);
	auto end = std::sregex_iterator();

	auto directive_iter = std::sregex_iterator(httpBlock.begin(), httpBlock.end(), directives);
	while(directive_iter != end)
	{
		std::string key = (*directive_iter)[1].str();
		std::string value = (*directive_iter)[2].str();
		http.directives[key] = value;
		++directive_iter;
	}

	while(server_iter != end)
	{
		ServerConf server;
		std::string block = (*server_iter)[1].str();
		parseServerBlock(block, server, route_pattern, directives);
		http.servers.push_back(server);
		++server_iter;
	}
}

ServerConf Config::parse()
{
	ServerConf server_confs;
	std::ifstream file(conf_file);
	if(!file.is_open())
		throw std::runtime_error("Can't open conf file");
	std::string line;
	ServerConf current_serv;
	RouteConf current_route;
	bool in_serv = false;
	bool in_route = false;

	std::string content((std::istreambuf_iterator<char>(file)),
	std::istreambuf_iterator<char>());

	std::regex events_block_pattern(R"(events\s*\{([^}]*)\})");
	std::regex http_block_pattern(R"(http\s*\{([^}]*)\})");
	std::regex server_block_pattern(R"(server\s*\{([^}]*)\})");
	std::regex route_block_pattern(R"(location\s+([^\s{]+)\s*\{([^}]*)\})");
	std::regex directive_pattern(R"((\w+)(?:\s+([^;]+))?;)");

	auto eventMatch = std::sregex_iterator(content.begin(), content.end(), events_block_pattern);
	if (eventMatch != std::sregex_iterator())
	{
		std::string eventBlock = (*eventMatch)[1].str();
		parseEventBlock(eventBlock);
	}
	auto httpMatch = std::sregex_iterator(content.begin(), content.end(), http_block_pattern);
	if (httpMatch != std::sregex_iterator())
	{
		std::string httpBlock = (*httpMatch)[1].str();
		parseHttpBlock(httpBlock, server_block_pattern, route_block_pattern, directive_pattern);
	}
	//clean up keys and values by trimming;

	//split up key search for settings: global, server, route, host, body, error_pages etc..
	//to fill out necessary data structs.
}


