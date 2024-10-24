/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/24 02:45:51 by mleibeng         ###   ########.fr       */
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
	if (size_t pos = host.find(':'); pos != std::string::npos) {
		hostname = host.substr(0, pos);
	}

	std::cout << "Looking for host: " << host << std::endl;
	std::cout << "Parsed hostname (without port): " << hostname << std::endl;
	std::cout << "Number of server configs: " << _config.getServerConfs().size() << std::endl;

	for (const auto& conf : _config.getServerConfs()) {
		std::cout << "Checking server config - hostname: " << conf.hostname << std::endl;
		std::cout << "Server names: ";
		for (const auto& name : conf.server_names) {
			std::cout << name << " ";
		}
		std::cout << std::endl;

		if (conf.hostname == hostname ||
			std::find(conf.server_names.begin(), conf.server_names.end(), hostname) != conf.server_names.end()) {
			server_conf = &conf;
			std::cout << "Found matching server config!" << std::endl;
			break;
		}
	}

	if (!server_conf) {
		std::cout << "No matching server config found!" << std::endl;
	}

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

		if (request.getUri().compare(0, path.length(), path) == 0) {
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

void		RequestHandler::handleRequest(Client& client)
{
	const ServerConf* server_conf = findServerConf(client.getRequest());
	if (!server_conf)
	{}
	const RouteConf* route_conf = findRouteConf(*server_conf, client.getRequest());
	if (!route_conf)
	{}
	if (!isMethodAllowed(*route_conf, client.getRequest().getMethod()))
	{}
	if (client.getRequest().getMethod() == "GET")
		handleGetRequest(client, *route_conf);
	else if (client.getRequest().getMethod() == "POST")
		handlePostRequest(client,*route_conf);
	else if (client.getRequest().getMethod() == "DELETE")
		handleDeleteRequest(client, *route_conf);
	else
	{}
}

void RequestHandler::sendDirListing(Client& client, const std::string& dir_path)
{
	HttpResponse response;
	std::stringstream html;

	html << "<html><head><title>Directory listing</title></head><body><h1>Directory listing for "
			<< client.getRequest().getUri() << "</h1><ul>";

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(dir_path))
		{
			std::string name = entry.path().filename().string();
			std::string path = client.getRequest().getUri();
			if (path.back() != '/') path += '/';

			html << "<li><a href=\"" << path << name << "\">"
					<< name << "</a></li>";
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Directory listing error: " << e.what() << std::endl;
		// serveErrorPage(client.getFd(), 500);
		return;
	}

	html << "</ul></body></html>";

	response.setStatus(StatusCode::OK);
	response.setBody(html.str());
	response.setMimeType("text/html");
	client.send_response(response.buildResponse());
}

void RequestHandler::sendFile(Client& client, const std::string& file_path)
{
	HttpResponse response;

	std::ifstream file(file_path, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << file_path << std::endl;
		return;
	}
	std::stringstream fileBuf;
	fileBuf << file.rdbuf();
	file.close();

	response.setStatus(StatusCode::OK);
	response.setBody(fileBuf.str());
	response.setMimeType(getFileExtension(file_path));
	client.send_response(response.buildResponse());
}

// und hier das argument nr2 so: ->  handleGetRequest(request, const RouteConf &route_conf)!
void		RequestHandler::handleGetRequest(Client& client, const RouteConf& route_conf)
{
	std::string file_path = route_conf.root + client.getRequest().getUri();
	std::cout << file_path << std::endl;
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
	if (std::filesystem::is_directory(file_path))
	{
		if (route_conf.dir_listing_active)
			sendDirListing(client, file_path);
		else if (!route_conf.default_file.empty())
		{
			if (file_path.back() != '/')
				file_path += '/';
			file_path += route_conf.default_file;
			sendFile(client, file_path);
		}
		// else
			// Errorpage 403 Forbidden.
	}
	else
		sendFile(client, file_path);
}

void		RequestHandler::handlePostRequest(Client& client, const RouteConf& route_conf)
{
	// std::string file_path = route_conf.root + request.getUri();
	(void)client;
	(void)route_conf;
	// HttpResponse response;

	// DIESE LOGIK MUSS REIN <- filedescriptor koennte ein issue sein. weil CGI schickt selber zurueck und baut keine Nachricht!!
	// if (!route_conf.cgi_extension.empty() && request.getUri().ends_with(route_conf.cgi_extension))
	//		handleCGI(int fd, file_path, request.getBody());
	// else
	//		normale handler logik!
	// client.send_response(response.buildResponse());
}

void		RequestHandler::handleDeleteRequest(Client& client, const RouteConf& route_conf)
{
	// std::string file_path = route_conf.root + request.getUri();

	// LOGIK:
	//  |
	//  V
	// if (std::filesystem::exists(file_path) && !std::filesystem::is_directory(file_path))
	// {
	// 	if (std::filesystem::remove(file_path))
	// 		client.send_response(HttpResponse(200, "text/plain", "File deleted successfully"));
	// 	else
	// 		serveErrorPage(client.get_fd(), 500); // Internal Server Error
	// }
	// else
	// 	serveErrorPage(client.get_fd(), 404); // Not

	(void)client;
	(void)route_conf;
	// HttpResponse response;
	// client.send_response(response.buildResponse());
}

std::string		getFileExtension(const std::string& filepath)
{
	return (std::filesystem::path(filepath).extension().string());
}
