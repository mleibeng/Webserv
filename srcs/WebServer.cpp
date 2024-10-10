/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:53 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/10 02:40:44 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
// #include "Loop.hpp"
// #include "Connection.hpp"

// Read in Config file
// setup
// start up loop

WebServer::WebServer(std::string &conf_file) : config(Config::parse(conf_file))
{
	config.print();
}
// loop = std::make_unique<Loop>();

void WebServer::start()
{
	//start listening on sockets/fds
	//start async threads for I/O operations
}

void WebServer::stop()
{
	// loop->stop();
	server_threading.wait();
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
