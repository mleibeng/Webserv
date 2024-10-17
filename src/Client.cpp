
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

const std::string Client::getRequest()
{
	return _request;
}

void Client::setHostname(const std::string &hostname)
{
	_hostname = hostname;
}

const std::string Client::getHostname()
{
	return _hostname;
}

void WebServer::handleClientRequest(int client_fd)
{
	// (void)client_fd;
	Client client(client_fd);

	std::cout << "request from " << client_fd << std::endl;
	client.read_request();

	const HttpRequest& requester(client.getRequest());

	client.setHostname(requester.getHeader("host"));

	const ServerConf *serverconf = nullptr;
	for (auto &conf : _config.getServerConfs())
	{
		if (conf._hostname == requester.getHeader("host") ||
			std::find(conf._server_names.begin(), conf._server_names.end(), client.getHostname()) != conf._server_names.end())
			serverconf = &conf;
	}

	// error handling

	const RouteConf *route_conf = nullptr;
	for (auto& [path, conf] : serverconf->_routes)
	{
		if (requester.getUri().compare(0, path.length(), path) == 0)
		{
			route_conf = &conf;
			break;
		}
	}



	std::cout << "response to " << client_fd << std::endl;
	client.send_response();
}


// Discussion topics i found:

// 1. maybe we build the client as the centerpiece of httprequest/response where these get saved? so that we can just hand over client?
// 	  because the response and request and message themselves need to be saved in something -> need to discuss!

// so i mean like this client ?
// {
// 	HttpRequest request;
// 	HttpResponse response;

// 	client_fd
// 	etc...
// }

// 2. the response also needs to send back mime.types!
// 3. do we sort the request itself (GET/POST/DELETE) in WebServer or in HttpRequestHandler?

/* void WebServer::handleClientRequest(int client_fd)
{
	INIT:
		Client client(client_fd);

		if (!client.read_request())
		{
			ServeErrorPage(client_fd, 400);
			return;
		}
		const HttpRequest& request = client.get_request(); // getter for the _request out of client class needed.
		-> maybe automatically parse in the http request constructor.

	 Server logic part here for finding the correct server and route for the response
	 -> select correct serverconfig
	 	-> compare server_hostname with request_hostname (Host: in the headers)
	 	-> or find servername fitting to request_servername
	 -> std::find functions after selecting the correct server config
	 	-> going through route map of the serverconf (std::unordered map[path, conf] : server_conf->_routes)
	 		with compare each route with request.route_path Headertype is -> (Referer: )
	 	-> if no route found -> error 404 not found
	 -> check if method is allowed
	 	-> ServeErrorPage(405 Forbidden in case of no)
	 Then call request.getMethod() = get/post/delete and those handle

	-> UP FOR DISCUSSION!
			-> either HttpRequestHandler:HandleRequest or WebServer::HandleRequest -> need to discuss what more efficient.
			switch:
				GET:

				POST:
					-> if content-type found in headers and content-type == multipart/form-data
						-> handleFileUpload
						-> maybe also search for uploaddir as the default route for default upload location
						and have it as an option in the parameters OR
						if not route found in config then set one in the fileuploadfunction!
					-> else
						-> handlePostRequest(client/httpRequest, *route_conf);
				DELETE:
					handleDeleteRequest(client/httpRequest, *route_conf);
				default:
					serveErrorPage(client_fd, 501 (not implemented));
}
*/

/*
HandleGetRequest(&client/httpRequest, &routeconf)
What we need:
1. file path is routeconf.root + client/httprequest.get_request.path
	example: -> root var/www/html + /favicon.ico
	(in the first line GET /favicon.ico HTTP/1.1)
{
	If routeconf_path == filesystem::is_directory(file_path)
	{
		-> if dir_listing is active
			-> sendDirListing(client/Httprequest, file_path);
		-> else if default file of routeconf is not empty
			-> file_path += route_conf._default_file
			-> sendFile(client, file_path);
		-> else serve Error page (403 Forbidden)}
	}
	else
		sendFile(client, file_path);
}
*/

/*
HandlePostRequest(&client/httpRequest, &route)
What we need:
{
	if route_conf._cgi_extension not empty
		-> if request.path ends with the cgi_extension like .php/.py etc..
			-> handleCGI(client_fd, route_conf.root + request.path, request.body)
		-> else
			-> handle post request as necessary in body/headers etc...
	after processing the post posting we send a confirmation back to client:
	-> client/requesthandler.sendResponse(HttpResponse(response_message));
}
*/

/*
HandleDeleteRequest(&client, &route)
What we need:
{
	1. file_path again
	if(std::filesystem::exists(file_path) and is not directory)
		std::filesystem::remove(file_path)
			client.sendresponse(HttpResponse(message like deletion successful!));
		else
			send errorpage.
	else if not found in filesystem serve 404 errorpage.
}
*/

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
// 	int in_pipe[2];
// 	int out_pipe[2];

// 	if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1)
// 	{
// 		perror("pipe error");
// 		return;
// 	}
// 	pid_t pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		close(in_pipe[0]);
// 		close(in_pipe[1]);
// 		close(out_pipe[0]);
// 		close(out_pipe[1]);
// 		return;
// 	}
// 	if (pid == 0)
// 	{
// 		close(out_pipe[0]);
// 		close(in_pipe[1]);

// 		dup2(in_pipe[0], STDIN_FILENO);
// 		dup2(out_pipe[1], STDOUT_FILENO);

// 		setenv("REQUEST_METHOD", "GET", 1);
// 		setenv("QUERY_STRING", query.c_str(), 1);
// 		setenv("CONTENT_LENGTH", "0", 1);

// 		execl("/usr/bin/php", "php", cgi_path.c_str(), NULL);
// 		perror("execl");
// 		exit(1);
// 	}
// 	else
// 	{
// 		close(out_pipe[1]);
// 		close(in_pipe[0]);

// 		char buffer[4096];
// 		int bytes_read;
// 		while ((bytes_read = read(out_pipe[0], buffer, sizeof(buffer))) > 0) {
// 			if (write(client_fd, buffer, bytes_read) == -1) {
// 				perror("write");
// 				break;
// 			}
// 		}
// 		close(out_pipe[0]);

// 		waitpid(pid, NULL, 0);
// 	}
// }
