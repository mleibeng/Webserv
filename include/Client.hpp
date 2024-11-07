/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/11/07 03:15:25 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
#define CLIENT_H

#include "HeaderIncludes.hpp"
#include "HttpRequest.hpp"
#include "Config.hpp"
#include "WebServer.hpp"

#define DEFAULT		"\033[0m"
#define RED			"\033[31m"
#define YELLOW		"\033[33m"

class Client {
	public:
		Client(int client_fd, const Config& config);
		~Client();

		Client() = delete;
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;

		const HttpRequest& getRequest() const;
		const int& getFd() const;

		ssize_t read_request();
		ssize_t send_response(const std::string& response_string);

		int getNumRedirects() const;

		void setRoute(const RouteConf* route);

		ssize_t processChunk(const std::string& chunky);

		//incrementers
		void increaseRedirectCount();
		void setBuffer(size_t buffersize);

		bool isFileUpload() const;
		bool needsRouteResolution() const;

		bool keepAlive() const { return _keep_alive; }
		const RouteConf* getRoute() const { return _route; }

	private:
		int _client_fd;
		const Config& _config;
		HttpRequest _request;
		size_t _buffersize;
		size_t redirect_count;
		const RouteConf *_route;
		bool _keep_alive;
};

#endif // CLIENT_H
