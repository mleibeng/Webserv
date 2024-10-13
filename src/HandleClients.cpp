
#include "WebServer.hpp"
#include "Client.hpp"

// HTTP Functionalities -> ErrorPages, FileUpload, CGIs
// Should to run through REQUEST and RESPONSE Classes
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Client::Client(int client_fd) : _client_fd(client_fd) {
}

Client::~Client() {
	close(_client_fd);
}

ssize_t Client::read_request() {
	ssize_t nbytes;
	char buffer[BUFFER_SIZE];

	do {
		nbytes = read(_client_fd, buffer, sizeof(buffer));
	} while (nbytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK));

	if (nbytes == -1) {
		std::cerr << RED << "read(): " << strerror(errno) << DEFAULT << std::endl;
	}
	else if (nbytes == 0) {
		close(_client_fd);
	}
	else {
		_request.assign(buffer, nbytes);
		std::cout << YELLOW << _request << DEFAULT << std::endl;
	}

	return nbytes;
}

ssize_t Client::send_response() {
	ssize_t nbytes;

	nbytes = write(_client_fd, _response.c_str(), _response.size());
	if (nbytes == -1) {
		std::cerr << RED << "write(): " << strerror(errno) << DEFAULT << std::endl;
	}

	return nbytes;
}

void WebServer::handleClientRequest(int client_fd)
{
	// (void)client_fd;
	Client client(client_fd);

	std::cout << "request from " << client_fd << std::endl;
	client.read_request();

	std::cout << "response to " << client_fd << std::endl;
	client.send_response();
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

// void WebServer::handleCGI(int client_fd, const std::string& cgi_path, const std::string& query)
// {
// 	(void)client_fd;
// 	(void)cgi_path;
// 	(void)query;
// }