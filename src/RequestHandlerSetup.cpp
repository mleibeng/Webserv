/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerSetup.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:28:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/31 22:32:46 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(const Config& config) : _config(config)
{
	loadErrorPages();
}

void RequestHandler::loadErrorPages() // now this works for more than 1 page but doesn't work for more than 1 server!
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

std::string extractHostname(const std::string& host)
{
	if (size_t pos = host.find(':'); pos != std::string::npos)
		return host.substr(0, pos);
	return host;
}

const ServerConf *RequestHandler::findServerConf(const HttpRequest &request)
{
	std::string hostname = extractHostname(request.getHeader("Host"));

	// std::cout << "Looking for host: " << host << std::endl;
	// std::cout << "Parsed hostname (without port): " << hostname << std::endl;
	// std::cout << "Number of server configs: " << _config.getServerConfs().size() << std::endl;

	for (const auto& conf : _config.getServerConfs())
	{
		// std::cout << "Checking server config - hostname: " << conf.hostname << std::endl;
		// std::cout << "Server names: ";
		// for (const auto& name : conf.server_names)
		// 	std::cout << name << " ";
		// std::cout << std::endl;

		if (conf.hostname == hostname ||
			std::find(conf.server_names.begin(), conf.server_names.end(), hostname) != conf.server_names.end())
			return &conf;
	}
	// if (!server_conf)
		// std::cout << "No matching server config found!" << std::endl;
	return nullptr;
}

const RouteConf *RequestHandler::findRouteConf(const ServerConf &server_conf, const HttpRequest& request)
{
	const std::string& uri = request.getUri();
	const RouteConf *best_match = nullptr;
	size_t longest_match = 0;

	for (const auto& [path, route] : server_conf.routes)
	{
		if (path.length() <= uri.length() &&
		uri.compare(0, path.length(), path) == 0 &&
		path.length() > longest_match)
		{
			longest_match = path.length();
			best_match = &route;
		}
	}
	if (best_match)
		return best_match;
	else
	{
		if(server_conf.routes.count("/"))
			return &server_conf.routes.at("/");
		else
			return nullptr;
	}
}

bool startsWith(const std::string& str, const std::string& prefix)
{
	if (prefix.size() > str.size())
		return false;
	return str.compare(0, prefix.size(), prefix) == 0;
}

bool RequestHandler::isMethodAllowed(const RouteConf &route_conf, const std::string &method)
{
	if (std::find(route_conf.methods.begin(), route_conf.methods.end(), method) == route_conf.methods.end())
		return false;
	return true;
}

ParsedPath RequestHandler::parsePath(const RouteConf& route_conf, const HttpRequest& request)
{
	ParsedPath res;
	std::string uri = request.getUri();

	// split query from URI
	size_t query_point = uri.find('?');
	if (query_point != std::string::npos)
	{
		res.query = uri.substr(query_point + 1);
		uri = uri.substr(0, query_point);
	}

	//set phys path to default path of the route_conf
	res.phys_path = route_conf.root;
	if (res.phys_path.back() != '/')
		res.phys_path += '/';

	// build a file resource request path from default path in config file,
	// checking for leading "/" in the URI and preventing double "//" appending to the phys_path
	if (route_conf.path == "/")
	{
		if (uri != "/")
			res.phys_path += (uri[0] == '/') ? uri.substr(1) : uri;
	}// checks whether or not the uri already leads with route config path and cuts it out, because phys path already includes it.
	else if (uri.compare(0, route_conf.path.length(), route_conf.path) == 0)
	{
		std::string remain = uri.substr(route_conf.path.length());
		if (!remain.empty())
			res.phys_path += (remain[0] == '/') ? remain.substr(1) : remain;
	}

	return res;
}
