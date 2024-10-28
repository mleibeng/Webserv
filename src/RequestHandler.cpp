/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/28 22:05:05 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/// @brief Entrypoint and management function for sorting Requests
/// @param client client including the fd and request. Ultimately receives requests and sends the built responses.
void		RequestHandler::handleRequest(Client& client)
{
	const ServerConf* server_conf = findServerConf(client.getRequest());
	if (!server_conf)
	{
		serveErrorPage(client, 404);
		return;
	}
	const RouteConf* route_conf = findRouteConf(*server_conf, client.getRequest());
	if (!route_conf)
	{
		serveErrorPage(client, 404);
		return;
	}
	if (!isMethodAllowed(*route_conf, client.getRequest().getMethod()))
	{
		serveErrorPage(client, 405);
		return;
	}
	if (client.getRequest().getMethod() == "GET")
		handleGetRequest(client, *route_conf);
	else if (client.getRequest().getMethod() == "POST")
		handlePostRequest(client,*route_conf);
	else if (client.getRequest().getMethod() == "DELETE")
		handleDeleteRequest(client, *route_conf);
	else
		serveErrorPage(client, 501);
}

/// @brief Get request handling logic // CURRENTLY MISSING CGI AND ERRORPAGE SUPPORT!!!!
/// @param client client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param route_conf correct route configuration for the request. Hands over available resources, flags and path configurations
void		RequestHandler::handleGetRequest(Client& client, const RouteConf& route_conf)
{
	std::string uri = client.getRequest().getUri();
	std::string file_path = route_conf.root + uri;
	std::cout << file_path << std::endl;
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	std::string query = "";
	size_t query_point = file_path.find('?');
	if (query_point != std::string::npos)
	{
		query = uri.substr(query_point + 1);
		std::cout << query << "\n\n";
		file_path = file_path.substr(0, query_point);
		std::cout << file_path << "\n\n";
	}
	if (!route_conf.cgi_extension.empty())
	{
		handleCGI(client, file_path, query);
		return;
	}
	if (std::filesystem::is_directory(file_path))
	{
		if (!route_conf.default_file.empty())
		{
			if (file_path.back() != '/')
				file_path += '/';
			file_path += route_conf.default_file;
			sendFile(client, file_path);
		}
		else if (route_conf.dir_listing_active)
			sendDirListing(client, file_path);
		else
			serveErrorPage(client, 405);
	}
	else
		sendFile(client, file_path);
}

/// @brief Function to build a directory listing to send back to the client with hyperlinks to available resources
/// @param client serverside representation of the client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param dir_path built directory path configured out of route_config and request URI.
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
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cerr << "Directory listing error: " << e.what() << std::endl;
		serveErrorPage(client, 500);
		return;
	}

	html << "</ul></body></html>";

	sendResponse(client, html.str());
}

/// @brief Function to send back a static file response that was requested by the browser.
/// @param client  serverside representation of the client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param file_path built file path configured out of route_config and request URI
void RequestHandler::sendFile(Client& client, const std::string& file_path)
{
	HttpResponse response;

	std::ifstream file(file_path, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Could not open file in sendFile func: " << file_path << std::endl;
		serveErrorPage(client, 500);
		return;
	}
	std::stringstream fileBuf;
	fileBuf << file.rdbuf();
	file.close();

	response.setStatus(200);
	response.setBody(fileBuf.str());
	response.setMimeType(getFileExtension(file_path));
	client.send_response(response.buildResponse());
}

// und hier das argument nr2 so: ->  handleGetRequest(request, const RouteConf &route_conf)!
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

// UTILS

std::string		getFileExtension(const std::string& filepath)
{
	return (std::filesystem::path(filepath).extension().string());
}
