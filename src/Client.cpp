/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/10/28 20:23:02 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Client::Client(int client_fd) : _client_fd(client_fd) {
}

Client::~Client() {
	close(_client_fd);
}

ssize_t Client::read_request()
{
	ssize_t nbytes;
	char buffer[BUFFER_SIZE];
	std::string request;

	// do {
		nbytes = read(_client_fd, buffer, sizeof(buffer));
	// } while (nbytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK));

	if (nbytes == -1) {
		std::cerr << RED << "read(): " << strerror(errno) << DEFAULT << std::endl;
	}
	else if (nbytes == 0) {
		close(_client_fd);
	}
	else {
		request.assign(buffer, nbytes);
		std::cout << YELLOW << request << DEFAULT << std::endl;
		_request.parse(request);
	}
	return nbytes;
}

ssize_t Client::send_response(const std::string& response_string) {
	ssize_t nbytes;

	std::cout << response_string << std::endl;
	nbytes = write(_client_fd, response_string.c_str(), response_string.size());
	if (nbytes == -1) {
		std::cerr << RED << "write(): " << strerror(errno) << DEFAULT << std::endl;
	}

	return nbytes;
}

const int& Client::getFd() const
{
	return _client_fd;
}

const HttpRequest& Client::getRequest() const
{
	return _request;
}

























// /// @brief Sets up a default error message + error code in case no default error page path was given in the config file and otherwise outputs the content from the error page path
// /// @param error_code Error codes like 404, 500 etc..
// /// @return In case of set up default error pages provides content from there otherwise sends a standard white page with Error + error code
// std::string WebServer::getErrorPage(int error_code)
// {
// 	std::string default_error_message = "<html><body><h1>Error " + std::to_string(error_code) + "</h1></body></html>";

// 	auto it = error_pages.find(error_code);
// 	if (it != error_pages.end())
// 	{
// 		std::ifstream file(it->second);
// 		if (file)
// 			return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
// 	}
// 	return default_error_message;
// }

// /// @brief sends error response back to client through ResponseHandler which should run this function.
// /// @param client_fd fd of the connected client sending the request to our webserver
// /// @param error_code 404, 500, etc..
// void WebServer::serveErrorPage(int client_fd, int error_code)
// {
// 	std::string error_content = getErrorPage(error_code);
// 	std::string response = "HTTP/1.1 " + std::to_string(error_code) + " " +
// 							(error_code == 404 ? "Not Found" : "Error") + "\r\n" +
// 							"Content-Type: text/html\r\n" +
// 							"Content-Length: " + std::to_string(error_content.length()) + "\r\n" +
// 							"\r\n" +
// 							error_content;
// 	write(client_fd, response.c_str(), response.length());
// }

// // 1. Parse the multipart/form-data content -> should get handled by the Request Parser!!
// // 2. Extract the file data
// // 3. Generate a unique filename
// // 4. Write the file data to the upload directory
// void WebServer::handleFileUpload(int client_fd, const std::string& upload_dir)
// {
// 	char buffer[4096];
// 	ssize_t bytes_read;
// 	std::string filename = upload_dir + std::to_string(time(nullptr)); // ->for unique filename

// 	std::ofstream outfile(filename, std::ios::binary);
// 	while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0)
// 		outfile.write(buffer, bytes_read);
// 	outfile.close();

// 	// basically send confirmation back to the client that it got uploaded. -> should also be handled later by the HTTP Response Class
// 	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nFile uploaded successfully";
// 	write(client_fd, response.c_str(), response.length());
// }
