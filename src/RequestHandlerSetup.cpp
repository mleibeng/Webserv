/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerSetup.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:28:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/03 20:08:50 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/// @brief save configuration file into request handler object and loads appropriate error pages
/// @param config config file to save
RequestHandler::RequestHandler(const Config& config) : _config(config)
{
	loadErrorPages();
}

/// @brief loads error pages specified in configuration file !Currently only works for last server processed
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
				_error_pages[error_code] = filebuf.str(); // <--- need to modify this into a vector of unordered maps for multiple server blocks

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

/// @brief utilizes loaded error page to build and send a response to client
/// @param client client to send response back to
/// @param error_code error code to serve
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

/// @brief find host:port combination and split
/// @param host host:port combination to split
/// @return string including only the host portion
std::string extractHostname(const std::string& host)
{
	if (size_t pos = host.find(':'); pos != std::string::npos)
		return host.substr(0, pos);
	return host;
}

/// @brief find and extract correct server configuration out of vector of server configs for sent request
/// @param request request to parse and respond to
/// @return ServerConf struct with correct hostname/servername or nullptr in case of failure
const ServerConf *RequestHandler::findServerConf(const HttpRequest &request)
{
	std::string hostname = extractHostname(request.getHeader("Host"));

	for (const auto& conf : _config.getServerConfs())
	{
		if (conf.hostname == hostname ||
			std::find(conf.server_names.begin(), conf.server_names.end(), hostname) != conf.server_names.end())
			return &conf;
	}
	return nullptr;
}

/// @brief find longest matching string from uri to route.path
/// @param server_conf serverconfig to parse for routes
/// @param request request to process and compare to route
/// @return best matching route/default route if not found/nullptr if no default
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

/// @brief check whether or not a string starts with a certain prefix (basic c++20 string function)
/// @param str string to check
/// @param prefix prefix to check at start of string
/// @return true/false
bool startsWith(const std::string& str, const std::string& prefix)
{
	if (prefix.size() > str.size())
		return false;
	return str.compare(0, prefix.size(), prefix) == 0;
}

/// @brief check whether or not configured route allows request method to be processed
/// @param route_conf route to check
/// @param method to check
/// @return  true/false
bool RequestHandler::isMethodAllowed(const RouteConf &route_conf, const std::string &method)
{
	if (std::find(route_conf.methods.begin(), route_conf.methods.end(), method) == route_conf.methods.end())
		return false;
	return true;
}

/// @brief finds best complete path to file
/// @param route_conf route config to build start of route
/// @param request request to add resource requested
/// @return built path to resource
std::string RequestHandler::parsePath(const RouteConf& route_conf, const HttpRequest& request)
{
	std::string phys_path;
	std::string uri = request.getUri();

	// std::cout << "Uri: " << uri <<std::endl;
	//set phys path to default path of the route_conf
	phys_path = route_conf.root;
	if (phys_path.back() != '/')
		phys_path += '/';

	// std::cout << "Phys_path root: " << phys_path << std::endl;
	// build a file resource request path from default path in config file,
	// checking for leading "/" in the URI and preventing double "//" appending to the phys_path
	if (route_conf.path == "/")
	{
		if (uri != "/")
			phys_path += (uri[0] == '/') ? uri.substr(1) : uri;
		// std::cout << "Phys_path after // mod : " << phys_path << std::endl;
	}// checks whether or not the uri already leads with route config path and cuts it out, because phys path already includes it.
	else if (uri.compare(0, route_conf.path.length(), route_conf.path) == 0)
	{
		if (uri.length() == route_conf.path.length() || uri[route_conf.path.length()] == '/')
		{
			std::string remain = uri.substr(route_conf.path.length());
			// std::cout << "remain uri : " << remain << std::endl;
			if (!remain.empty())
				phys_path += (remain[0] == '/') ? remain.substr(1) : remain;
			// std::cout << "phys path after remain uri : " << phys_path << std::endl;
		}
		else
			phys_path += (uri[0] == '/' ? uri.substr(1) : uri);
	}

	return phys_path;
}
