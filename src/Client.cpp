/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/11/28 21:28:04 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// -----------------------------------------------------------------------------

Client::Client(int client_fd, const Config& config) : _client_fd(client_fd),
													  _config(config),
													  _buffersize(std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_HEADER_SIZE)) + std::get<size_t>(config.getGlobalConf(GlobalConf::ConfigKey::MAX_BODY_SIZE))),
													  redirect_count(0), _route(nullptr), _keep_alive(true), _best_path()
{}

Client::~Client()
{
	close(_client_fd);
}

const	std::string& Client::getResponseString() const
{
	return _response_to_send;
}

void	Client::setResponseString(const std::string& built_response)
{
	_response_to_send = built_response;
}

void Client::setBuffer(size_t buffersize)
{
	_buffersize = buffersize;
}

/// @brief reads in the clientside data sent from the webbrowser
/// @return returns length of request or -1 in case of error
ssize_t Client::read_request()
{
	std::vector<char> buffer(_buffersize + 1);
	ssize_t total_read = 0;

	while (true) {
		ssize_t bytes = read(_client_fd,
							buffer.data() + total_read,
							buffer.size() - total_read);

		if (bytes <= 0)
		{
			_keep_alive = false;
			break;
		}

		total_read += bytes;
		if (total_read >= static_cast<ssize_t>(_buffersize)) {
			return -1;  // Request too large
		}
	}

	if (total_read > 0)
	{
		std::string request(buffer.data(), total_read);
		bool ok = _request.parse(request);
		if (!ok)
			return -1;

		// Update keep-alive based on HTTP headers
		_keep_alive = checkKeepAliveHeaders();
	}

	return total_read;
}

int Client::getNumRedirects() const
{
	return (redirect_count);
}

void Client::increaseRedirectCount()
{
	redirect_count++;
}

void Client::setRoute(const RouteConf* route)
{
	_route = route;
}

bool Client::keepAlive() const
{return _keep_alive;}

bool Client::checkKeepAliveHeaders()
{
	auto connection = _request.getHeader("Connection");
	if (!connection.empty())
		return std::strcmp(connection.c_str(), "close") != 0;
	return _request.getHttpVersion() == "HTTP/1.1";
}

const RouteConf* Client::getRoute() const
{return _route;}

const std::string& Client::getBestPath() const
{return _best_path;}

int Client::setCourse()
{
	const ServerConf* server_conf = findServerConf(_request);
	if (!server_conf)
		return 404;

	const RouteConf* route_conf = findRouteConf(*server_conf, _request);
	if (!route_conf)
		return 404;

	_route = route_conf;
	_best_path = parsePath(*route_conf, _request);

	return 0;
}

/// @brief sends a response back to the client side
/// @param response_string std::string response to send
/// @return returns length of string sent or -1 in case of write error
ssize_t Client::send_response(const std::string& response_string)
{
	ssize_t total_sent = 0;
	size_t remaining = response_string.size();

	while (total_sent < static_cast<ssize_t>(response_string.size())) {
		ssize_t sent = write(_client_fd,
							response_string.c_str() + total_sent,
							remaining);

		if (sent <= 0)
			return -1;

		total_sent += sent;
		remaining -= sent;
	}

	close(_client_fd);

	return total_sent;
}

/// @brief get fd of client
/// @return returns an integer
const int& Client::getFd() const
{
	return _client_fd;
}

/// @brief get parsed request as object
/// @return returns an HttpRequest object
const HttpRequest& Client::getRequest() const
{
	return _request;
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
const ServerConf *Client::findServerConf(const HttpRequest &request)
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
const RouteConf *Client::findRouteConf(const ServerConf &server_conf, const HttpRequest& request)
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
bool Client::isMethodAllowed(const RouteConf &route_conf, const std::string &method)
{
	if (std::find(route_conf.methods.begin(), route_conf.methods.end(), method) == route_conf.methods.end())
		return false;
	return true;
}

/// @brief finds best complete path to file
/// @param route_conf route config to build start of route
/// @param request request to add resource requested
/// @return built path to resource
std::string Client::parsePath(const RouteConf& route_conf, const HttpRequest& request)
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
