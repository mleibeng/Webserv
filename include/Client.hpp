/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/12/06 16:30:21 by mleibeng         ###   ########.fr       */
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

		const HttpRequest&	getRequest() const;
		HttpRequest& 		getRequest();
		const std::string&	getResponseString() const;
		void				setResponseString(const std::string& built_response);
		int					getFd() const;
		bool				hasResponse();

		ssize_t				read_request();
		ssize_t				send_response(const std::string& response_string);

		int					getNumRedirects() const;

		void				setRoute(const RouteConf* route);

		//incrementers
		void				increaseRedirectCount();
		void				setBuffer(size_t buffersize);

		//connection checkers
		bool				keepAlive() const;
		bool				checkKeepAliveHeaders();

		//route mngmt
		const std::string&	getName() const;
		int					setCourse();
		const RouteConf*	getRoute() const;
		const std::string&	getBestPath() const;
		const std::string&	getRequestToAppend() const;
		const ServerConf	*findServerConf(const HttpRequest &request);
		const RouteConf		*findRouteConf(const ServerConf &server_conf, const HttpRequest& request);
		std::string			parsePath(const RouteConf& route_conf, const HttpRequest& request);
		int					isMethodAllowed(const RouteConf &route_conf, const std::string& method);
		std::string 		generateUniqueName();

	private:
		static int		client_counter;
		std::string		_client_name;
		int				_client_fd;
		const Config&	_config;
		HttpRequest		_request;
		size_t			_buffersize;
		size_t			redirect_count;
		const RouteConf	*_route;
		bool			_keep_alive;
		std::string		_best_path;
		std::string		_request_to_append;
		std::string		_response_to_send;
};

#endif // CLIENT_H
