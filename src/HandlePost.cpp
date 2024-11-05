/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlePost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:54:25 by mott              #+#    #+#             */
/*   Updated: 2024/11/05 21:04:56 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/// @brief
/// @param client
/// @param route_conf
/// @param parsed
void	RequestHandler::handlePostRequest(Client& client, const RouteConf& route_conf, const std::string& parsed)
{
	const std::string& content_type = client.getRequest().getHeader("Content-Type");
	const std::string& body = client.getRequest().getBody();
	// std::cout << RED << "Content-Type: " << content_type << RESET << std::endl;
	// std::cout << RED << "Body: " << body << RESET << std::endl;

	if (content_type.find("multipart/form-data") != std::string::npos)
	{
		handleFileUpload(client, content_type, body);
	}
	else if (content_type.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		handleFormSubmission(client, body);
	}
	else if (!route_conf.cgi_extension.empty() && getFileExtension(parsed) == route_conf.cgi_extension)
	{
		handleCGI(client, parsed);
	}
	else
	{
		std::cout << RED << "Content-Type not supported" << RESET << std::endl;
	}
}

void	RequestHandler::handleFileUpload(Client& client, const std::string& content_type, const std::string& body)
{
	size_t pos_boundary = content_type.find("boundary=");
	std::string boundary = content_type.substr(pos_boundary + 9);
	// std::cout << RED << "boundary: " << boundary << RESET << std::endl;

	size_t file_start = body.find(boundary) + boundary.size() + 2 ;	// "\r\n"
	size_t file_end = body.find(boundary, file_start) - 4;			// "\r\n--"

	if (file_start != std::string::npos && file_end != std::string::npos) {
		size_t content_start = body.find("\r\n\r\n", file_start) + 4;
		std::string file_data = body.substr(content_start, file_end - content_start);

		std::ofstream file("/workspace/42/projects/5_webserv/html_pages/uploads/uploaded_file", std::ios::binary);
		if (file) {
			file.write(file_data.data(), file_data.size());
			file.close();

			HttpResponse response;
			response.setStatus(201);
			response.setBody("upload successful");
			response.setMimeType(".html");
			client.send_response(response.buildResponse());
		}
		else {
			std::cout << RED << "KO 1: " << std::strerror(errno) << RESET << std::endl;
		}
	}
	else {
		std::cout << RED << "KO 2: " << std::strerror(errno) << RESET << std::endl;
	}
}

void	RequestHandler::handleFormSubmission(Client& client, const std::string& body)
{
	size_t pos_name = body.find("name=");
	size_t pos_message = body.find("message=");

	std::string name = body.substr(pos_name + 5, pos_message - (pos_name + 5) - 1);
	std::string message = body.substr(pos_message + 8);
	// std::cout << RED << "name: " << name << RESET << std::endl;
	// std::cout << RED << "message: " << message << RESET << std::endl;

	HttpResponse response;
	response.setStatus(201);
	response.setBody("name: " + name + "<br>" + "message: " + message);
	response.setMimeType(".html");
	client.send_response(response.buildResponse());
}
