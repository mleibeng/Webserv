/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/10 23:16:59 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
// #include "Loop.hpp"
// #include "Connection.hpp"

// 1. Read in Config file
// 2. setup
// 3. start up loop

/// @brief Webserver Constructor -> Parses and prints config data and sets running to false
/// @param conf_file Saves data structures and values read in from the config file
WebServer::WebServer(const std::string &conf_file) : config(Config::parse(conf_file)), running(false)
{
	config.print();
}

/// @brief Stops the Server, cleaning up resources cleanly
WebServer::~WebServer()
{
	stop();
}

/// @brief Provides eventLoop functionality, renaming and splitting up of functionality in the future
void WebServer::acceptConnections()
{
	// while (running)
	// {
	// 	//here comes accept stuff into;
	// }
}

/// @brief initializes epoll_fd and sets up the listening socket fds. Also loads the error pages into the WebServer class
void WebServer::initialize()
{
	// set up epoll_fd using epoll_create here
	setupListeners();

	for (const auto& server : config.getServerConfs())
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
					error_pages[error_code] = page_path;
			}
		}
	}
}

int WebServer::createNonBlockingSocket()
{
	int fd = 0;
	// here comes socket call into and setting of flags.
	return fd;
}

void WebServer::setupListeners()
{
	for(const auto& server: config.getServerConfs())
	{
		std::vector<int> ports = {server.port};
		for (const auto& route : server.routes)
		{
			if (route.second.port.has_value() && route.second.port.value() != 0 && route.second.port.value() != server.port)
				ports.push_back(route.second.port.value());
		}

		// for (const auto& port : ports)
		// {
		// 	std::string server_key = server.hostname + ":" + std::to_string(server.port);

		// 	int fd = createNonBlockingSocket();
		// 	struct sockaddr_in addr;
		// 	addr.sin_family = AF_INET;
		// 	addr.sin_addr.s_addr = inet_addr(server.hostname.c_str());
		// 	addr.sin_port = htons(server.port);
		// }
		//etc etc etc... now we bind ports and start the listen process etc..
		//and then just pushback into server_listeners[server_key].pushback[fd] so we have a vector of running listening sockets.
		// then we add fd to epoll using EPOLLIN
	}
}

void WebServer::start()
{
	if (server_listeners.empty())
		throw std::runtime_error("No listeners set up.");
	running = true;
	// server_thread = std::async(std::launch::async, &WebServer::acceptConnections, this); -> this is the main thread running our connections
	// so we can use stop here for example by waiting for command input using some kind of input request.
}

void WebServer::stop()
{
	running = false;
	// if server thread is valid then -> server_thread.wait();
	// need to also close the listening sockets using their fd
}

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Sets up a default error message + error code in case no default error page path was given in the config file and otherwise outputs the content from the error page path
/// @param error_code Error codes like 404, 500 etc..
/// @return In case of set up default error pages provides content from there otherwise sends a standard white page with Error + error code
std::string WebServer::getErrorPage(int error_code)
{
	std::string default_error_message = "<html><body><h1>Error " + std::to_string(error_code) + "</h1></body></html>";

	auto it = error_pages.find(error_code);
	if (it != error_pages.end())
	{
		std::ifstream file(it->second);
		if (file)
			return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
	return default_error_message;
}

/// @brief sends error response back to client through ResponseHandler which should run this function.
/// @param client_fd fd of the connected client sending the request to our webserver
/// @param error_code 404, 500, etc..
void WebServer::serveErrorPage(int client_fd, int error_code)
{
	std::string error_content = getErrorPage(error_code);
	std::string response = "HTTP/1.1 " + std::to_string(error_code) + " " +
							(error_code == 404 ? "Not Found" : "Error") + "\r\n" +
							"Content-Type: text/html\r\n" +
							"Content-Length: " + std::to_string(error_content.length()) + "\r\n" +
							"\r\n" +
							error_content;
	write(client_fd, response.c_str(), response.length());
}

// 1. Parse the multipart/form-data content -> should get handled by the Request Parser!!
// 2. Extract the file data
// 3. Generate a unique filename
// 4. Write the file data to the upload directory
void WebServer::handleFileUpload(int client_fd, const std::string& upload_dir)
{
	char buffer[4096];
	ssize_t bytes_read;
	std::string filename = upload_dir + std::to_string(time(nullptr)); // ->for unique filename

	std::ofstream outfile(filename, std::ios::binary);
	while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0)
		outfile.write(buffer, bytes_read);
	outfile.close();

	// basically send confirmation back to the client that it got uploaded. -> should also be handled later by the HTTP Response Class
	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nFile uploaded successfully";
	write(client_fd, response.c_str(), response.length());
}

void WebServer::handleCGI(int client_fd, const std::string& cgi_path, const std::string& query)
{
	(void)client_fd;
	(void)cgi_path;
	(void)query;
}
