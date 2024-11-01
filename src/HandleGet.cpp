/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:39:54 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 02:03:20 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"


/// @brief Get request handling logic
/// @param client client including the fd and request. Ultimately receives requests and sends the built responses.
/// @param route_conf correct route configuration for the request. Hands over available resources, flags and path configurations
void		RequestHandler::handleGetRequest(Client& client, const RouteConf& route_conf, const std::string& parsed)
{
	// std::cout << "phys path: "<< parsed << " query: " << client.getRequest().getQuery() << std::endl;

	// Case 1: Specific file check
	if (std::filesystem::exists(parsed) && !std::filesystem::is_directory(parsed))
	{
		// std::cout << "option 1 direct resource request" << std::endl;
		std::string extension = getFileExtension(parsed);
		if (!extension.empty() && extension == route_conf.cgi_extension)
		{
			// std::cout << "option 1 php cgi" << std::endl;
			handleCGI(client, parsed);
			return;
		}
		// std::cout << "option 1 static file" << std::endl;
		sendFile(client, parsed);
		return;
	}

	// Case 2 Directory handling
	std::string path_check = parsed;

	// probably unecessary now since being utilizied in parsed already.
	// if (path_check.back() != '/')
	// 	path_check += '/';

	if (std::filesystem::is_directory(path_check))
	{
		// std::cout << "option 2: default file" << std::endl;
		// Default file check : Either PHP or static
		if (!route_conf.default_file.empty())
		{
			std::string default_path = path_check + route_conf.default_file;
			// std::cout << "default path opt 2: " << default_path << std::endl;
			if (std::filesystem::exists(default_path))
			{
				std::string extension = getFileExtension(default_path);
				if (!route_conf.cgi_extension.empty() && extension == route_conf.cgi_extension)
				{
					// std::cout << "option 2 cgi handler called" << std::endl;
					handleCGI(client, default_path);
					return;
				}
				sendFile(client, default_path);
				return;
			}
		}
		// Directory listing
		if (route_conf.dir_listing_active)
		{
			// std::cout << "option 3: Dir_listing" << std::endl;
			sendDirListing(client, path_check);
			return;
		}
		serveErrorPage(client, 403);
		return;
	}
	// std::cout << "option 4: Not found" << std::endl;
	// Case not found
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