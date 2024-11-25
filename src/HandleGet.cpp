/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:39:54 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/25 17:03:22 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/// @brief Get request handling logic
/// @param client client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param route_conf correct route configuration for the request. Hands over available resources, flags and path configurations
void		RequestHandler::handleGetRequest(Client& client)
{
	const RouteConf* route_conf = client.getRoute();
	const std::string& parsed = client.getBestPath();

	// Case 1: Specific file check
	if (std::filesystem::exists(parsed) && !std::filesystem::is_directory(parsed))
	{
		// std::cout << "case 1" << std::endl;
		std::string extension = getFileExtension(parsed);
		if (!extension.empty() && (extension == ".php" || extension == ".py"))
			return handleCGI(client, parsed);
		return sendFile(client, parsed);
	}

	// Case 2 Directory handling
	// std::cout << "case 2" << std::endl;
	if (std::filesystem::is_directory(parsed))
	{
		// Default file check : Either PHP or static
		if (!route_conf->default_file.empty())
		{
			std::string default_path = parsed + route_conf->default_file;
			if (std::filesystem::exists(default_path))
			{
				std::string extension = getFileExtension(default_path);
				if (!route_conf->cgi_extension.empty() && extension == route_conf->cgi_extension)
					return handleCGI(client, default_path);
				return sendFile(client, default_path);
			}
		}
		// std::cout << "case 3" << std::endl;
		// Directory listing
		if (route_conf->dir_listing_active)
			return sendDirListing(client, parsed);
		return serveErrorPage(client, 403);
	}
	// std::cout << "case 4 error" << std::endl;
	serveErrorPage(client, 404);
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

	response.setStatus(200);
	response.setBody(html.str());
	response.setMimeType(getFileExtension(".html"));
	client.send_response(response.buildResponse());
}

/// @brief Function to send back a static file response that was requested by the browser.
/// @param client  serverside representation of the client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param file_path built file path configured out of route_config and request URI
void RequestHandler::sendFile(Client& client, const std::string& file_path)
{
	HttpResponse response;
	const HttpRequest& request = client.getRequest();

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

	//Cookie tryout
	int visit_count;
	std::string visit_counter = request.getCookie("visit_count");
	if (visit_counter.empty())
		visit_count = 1;
	else
	{
		visit_count = std::stoi(visit_counter);
		if (getFileExtension(file_path) == ".html")
			visit_count++;
	}
	response.setStatus(200);
	response.setCookie("visit_count", std::to_string(visit_count));
	response.setCookie("lastVisit", getTime());
	response.setBody(fileBuf.str());
	response.setMimeType(getFileExtension(file_path));
	client.send_response(response.buildResponse());
}
