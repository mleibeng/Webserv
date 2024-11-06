/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlePost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:54:25 by mott              #+#    #+#             */
/*   Updated: 2024/11/06 14:44:12 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

void	RequestHandler::handlePostRequest(Client& client, const RouteConf& route_conf, const std::string& parsed)
{
	const std::string& content_type = client.getRequest().getHeader("Content-Type");
	const std::string& body = client.getRequest().getBody();

	if (content_type.find("multipart/form-data") != std::string::npos) {
		handleFileUpload(client, content_type, body);
	}
	else if (content_type.find("application/x-www-form-urlencoded") != std::string::npos) {
		handleFormSubmission(client, body);
	}
	else if (!route_conf.cgi_extension.empty() && getFileExtension(parsed) == route_conf.cgi_extension) {
		handleCGI(client, parsed);
	}
	else {
		std::cout << RED << "Content-Type not supported" << RESET << std::endl;
	}
}

void	RequestHandler::handleFileUpload(Client& client, const std::string& content_type, const std::string& body)
{
	std::string boundary = extractBoundary(content_type);
	if (boundary.empty()) {
		return serveErrorPage(client, 400); // Boundary not found in content type
	}

	std::string file = extractFile(body, boundary);
	if (file.empty()) {
		return serveErrorPage(client, 400); // File not found
	}

	std::string filename = extractFilename(file);
	if (filename.empty()) {
		return serveErrorPage(client, 400); // Filename not found
	}

	std::string file_data = extractFileData(file);
	if (file_data.empty()) {
		return serveErrorPage(client, 400); // File data not found
	}

	std::filesystem::path upload_dir = std::filesystem::current_path() / "html_pages/uploads";
	std::filesystem::path file_path = upload_dir / filename;
	if (std::filesystem::exists(file_path)) {
		return serveErrorPage(client, 409); // File already exists
	}

	std::ofstream new_file(file_path, std::ios::binary);
	if (new_file) {
		new_file.write(file_data.data(), file_data.size());
		new_file.close();

		HttpResponse response;
		response.setStatus(201);
		response.setBody("upload successful");
		response.setMimeType(".html");
		client.send_response(response.buildResponse());
	}
	else {
		return serveErrorPage(client, 500); // Internal Server Error if file creation fails
	}
}

std::string RequestHandler::extractBoundary(const std::string& content_type)
{
	size_t pos = content_type.find("boundary=");

	if (pos != std::string::npos) {
		return content_type.substr(pos + 9);
	}
	return "";
}

std::string RequestHandler::extractFile(const std::string& body, const std::string& boundary)
{
	size_t file_start = body.find(boundary) + boundary.size() + 2;	// +2 == \r\n
	size_t file_end = body.find(boundary, file_start) - 4;			// -4 == \r\n--

	if (file_start != std::string::npos && file_end != std::string::npos) {
		return body.substr(file_start, file_end - file_start);
	}
	return "";
}

std::string RequestHandler::extractFilename(const std::string& file)
{
	size_t filename_start = file.find("filename=\"") + 10;	// +10 == filename="
	size_t filename_end = file.find("\"", filename_start);

	if (filename_start != std::string::npos && filename_end != std::string::npos) {
		return file.substr(filename_start, filename_end - filename_start);
	}
	return "";
}

std::string RequestHandler::extractFileData(const std::string& file)
{
	size_t content_start = file.find("\r\n\r\n") + 4;	// +4 == \r\n\r\n

	if (content_start != std::string::npos) {
		return file.substr(content_start);
	}
	return "";
}

void	RequestHandler::handleFormSubmission(Client& client, const std::string& body)
{
	size_t pos_name = body.find("name=");
	size_t pos_message = body.find("message=");

	std::string name = body.substr(pos_name + 5, pos_message - (pos_name + 5) - 1);
	std::string message = body.substr(pos_message + 8);

	HttpResponse response;
	response.setStatus(201);
	response.setBody("name: " + name + "<br>" + "message: " + message);
	response.setMimeType(".html");
	client.send_response(response.buildResponse());
}
