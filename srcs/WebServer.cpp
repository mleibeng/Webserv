/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/10 15:16:05 by marvinleibe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
// #include "Loop.hpp"
// #include "Connection.hpp"

// Read in Config file
// setup
// start up loop

WebServer::WebServer(const std::string &conf_file) : config(Config::parse(conf_file)), running(false)
{
	config.print();
}
// loop = std::make_unique<Loop>();

WebServer::~WebServer()
{
	stop();
}

int WebServer::createNonBlockingSocket()
{
	// int fd;
	// // here comes socket call into and setting of flags.
	return 0;
}

void WebServer::setupListeners()
{
	// for(const auto& server: config.getServerConfs())
	// {
	// 	std::string server_key = server.hostname + ":" + std::to_string(server.port);
		
	// 	int fd = createNonBlockingSocket();
	// 	struct sockaddr_in addr;
	// 	addr.sin_family = AF_INET;
	// 	addr.sin_addr.s_addr = inet_addr(server.hostname.c_str());
	// 	addr.sin_port = htons(server.port);
	// 	//etc etc etc... now we bind ports and start the listen process etc.. 
	// 	//and then just pushback into server_listeners[server_key].pushback[fd] so we have a vector of running listening sockets.
	// }
}

void WebServer::acceptConnections()
{
	// while (running)
	// {
	// 	//here comes accept stuff into;
	// }
}

void WebServer::initialize()
{
	setupListeners();
}

void WebServer::start()
{
	if (server_listeners.empty())
		throw std::runtime_error("No listeners set up.");
	running = true;
	// server_thread = std::async(std::launch::async, &WebServer::acceptConnections, this);
	//start listening on sockets/fds
	//start async threads for I/O operations
}

void WebServer::stop()
{
	running = false;
	// server_thread.wait();
	// need to also close the listening sockets using their fd
}


/*	Not correct!!!! but basic graph! -> This doesnt show how the loops should take and pass info, not state management etc..
    this is just for a basic request!
    +---------------+
	|   WebServer   |
	+---------------+
	| - ConfigParser |
	| - EventLoop    |
	| - listen_fds   |
	+---------------+
			|
			|
			v
	+---------------+
	|   EventLoop   |
	+---------------+
	| - kqueue_fd   |
	| - fd_map      |
	| - timers      |
	| - run()       |
	| - stop()      |
	| - addFD()     |
	| - removeFD()  |
	| - updateKqueue()|
	+---------------+
			|
			|
			v
	+---------------+
	|   Connection  |
	+---------------+
	| - fd          |
	| - request     |
	| - response    |
	| - buffer      |
	| - handleRead()|
	| - handleWrite()|
	| - handleError()|
	| - process_req()|
	| - send_resp()  |
	+---------------+
			|
			|
			v
	+---------------+
	|   HttpRequest |
	+---------------+
	| - method      |
	| - path        |
	| - query       |
	| - body        |
	| - headers     |
	| - parse()     |
	+---------------+
			|
			|
			v
	+---------------+
	|  HttpResponse |
	+---------------+
	| - status_code |
	| - headers     |
	| - body        |
	| - toString()  |
	+---------------+
			|
			|
			v
	+---------------+
	|   CGIHandler  |
	+---------------+
	| - script_path |
	| - execute()   |
	+---------------+*/
