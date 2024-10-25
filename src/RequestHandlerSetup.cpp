/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerSetup.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:28:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/25 22:25:29 by mleibeng         ###   ########.fr       */
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
		if (!std::filesystem::exists(server.default_error_pages))
			throw std::runtime_error("Error pages not defined!");
		for (const auto& file : std::filesystem::directory_iterator(server.default_error_pages))
		{
			if (!file.is_regular_file())
				continue;

			std::string filename = file.path().filename().string();

			try
			{
				size_t dot = filename.find('.');
				if (dot == std::string::npos)
					continue;
				int error_code = std::stoi(filename.substr(0, dot));

				std::ifstream content(file.path(), std::ios::binary);
				if (!content.is_open())
					throw std::runtime_error("cannot open error page");

				std::stringstream filebuf;
				filebuf << content.rdbuf();
				// std::cout << filebuf.str() << std::endl; // debugging
				_error_pages[error_code] = filebuf.str();

				content.close();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				continue;
			}
		}
	}
}

void RequestHandler::serveErrorPage(Client& client, int error_code)
{
	HttpResponse response;

	response.setStatus(error_code);
	response.setBody(_error_pages[error_code]);
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