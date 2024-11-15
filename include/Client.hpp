/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/11/15 03:31:06 by mleibeng         ###   ########.fr       */
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

		//incrementers
		void increaseRedirectCount();
		void setBuffer(size_t buffersize);

		//connection checkers
		bool keepAlive() const;
		bool checkKeepAliveHeaders();

		//route mngmt
		int setCourse();
		const RouteConf* getRoute() const;
		const std::string& getBestPath() const;
		const ServerConf *findServerConf(const HttpRequest &request);
		const RouteConf *findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		std::string parsePath(const RouteConf& route_conf, const HttpRequest& request);
		bool isMethodAllowed(const RouteConf &route_conf, const std::string& method);

	private:
		int _client_fd;
		const Config& _config;
		HttpRequest _request;
		size_t _buffersize;
		size_t redirect_count;
		const RouteConf *_route;
		bool _keep_alive;
		std::string _best_path;
};

#endif // CLIENT_H
