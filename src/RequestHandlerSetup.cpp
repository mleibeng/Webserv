/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerSetup.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:28:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/28 01:32:36 by mleibeng         ###   ########.fr       */
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
	// client.send_response(response.buildResponse());
	client.queueResponse(response.buildResponse());
}

RequestHandler::~RequestHandler()
{
}
