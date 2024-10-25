/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerSetup.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:28:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/25 20:29:09 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(const Config& config) : _config(config)
{
	loadErrorPages();
}

void RequestHandler::loadErrorPages() // this probably only works if we have 1 singular error pages...
{
	for (const auto &server : _config.getServerConfs())
	{
		if (!server.default_error_pages.empty())
		{
			std::ifstream file(server.default_error_pages);
			std::string line;
			while (std::getline(file, line))
			{
				std::istringstream iss(line);
				int error_code;
				std::string page_path;
				if (iss >> error_code >> page_path)
					_error_pages[error_code] = page_path;
			}
		}
	}
}

void RequestHandler::serveErrorPage(Client& client, int error_code)
{
	HttpResponse response;

	std::ifstream file(_error_pages[error_code], std::ios::binary);
	if (!file.is_open())
	{
		response.setStatus(error_code);
		response.setBody("Error " + std::to_string(error_code));
		response.setMimeType(getFileExtension("text/plain"));
		client.send_response(response.buildResponse());
	}
	std::stringstream fileBuf;
	fileBuf << file.rdbuf();
	file.close();

	response.setStatus(error_code);
	response.setBody(fileBuf.str());
	response.setMimeType(getFileExtension("text/html"));
	client.send_response(response.buildResponse());
}

RequestHandler::~RequestHandler()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}
const ServerConf *RequestHandler::findServerConf(const HttpRequest &request)
{
	const ServerConf *server_conf = nullptr;
	std::string host = request.getHeader("Host");

	// Split host and port
	std::string hostname = host;
	if (size_t pos = host.find(':'); pos != std::string::npos)
		hostname = host.substr(0, pos);

	std::cout << "Looking for host: " << host << std::endl;
	std::cout << "Parsed hostname (without port): " << hostname << std::endl;
	std::cout << "Number of server configs: " << _config.getServerConfs().size() << std::endl;

	for (const auto& conf : _config.getServerConfs())
	{
		std::cout << "Checking server config - hostname: " << conf.hostname << std::endl;
		std::cout << "Server names: ";
		for (const auto& name : conf.server_names)
			std::cout << name << " ";
		std::cout << std::endl;

		if (conf.hostname == hostname ||
			std::find(conf.server_names.begin(), conf.server_names.end(), hostname) != conf.server_names.end())
		{
			server_conf = &conf;
			std::cout << "Found matching server config!" << std::endl;
			break;
		}
	}
	if (!server_conf)
		std::cout << "No matching server config found!" << std::endl;
	return server_conf;
}

const RouteConf *RequestHandler::findRouteConf(const ServerConf &server_conf, const HttpRequest& request)
{
	const RouteConf *route_conf = nullptr;
	std::cout << "Number of routes: " << server_conf.routes.size() << std::endl;
	for (const auto& pair : server_conf.routes)
	{
		const std::string& path = pair.first;
		const RouteConf& conf = pair.second;

		// Debug print
		std::cout << "Checking path: " << path << std::endl;

		if (request.getUri().compare(0, path.length(), path) == 0)
		{
			route_conf = &conf;
			break;
		}
	}

	return route_conf;
}

bool RequestHandler::isMethodAllowed(const RouteConf &route_conf, const std::string &method)
{
	if (std::find(route_conf.methods.begin(), route_conf.methods.end(), method) == route_conf.methods.end())
		return false;
	return true;
}