/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnv.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 00:55:12 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 01:41:58 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::CGIEnv::CGIEnv(const HttpRequest& request, const std::string& cgi_path, const CGIHandler& handler)
{
	setEnviron(request, cgi_path, handler);
}

RequestHandler::CGIEnv::~CGIEnv()
{
	env_array.clear();
	env_bufs.clear();
}

char** RequestHandler::CGIEnv::getEnv()
{return env_array.data();}

void RequestHandler::CGIEnv::setEnviron(const HttpRequest& request, const std::string& cgi_path, const CGIHandler& handler)
{
	addEnvStuff("REQUEST_METHOD", request.getMethod());
	addEnvStuff("QUERY_STRING", request._query);
	addEnvStuff("SCRIPT_FILE", cgi_path);
	addEnvStuff("REDIRECT_STATUS", "200");
	addEnvStuff("PATH_INFO", "");
	addEnvStuff("SERVER_SOFTWARE", "WebServer/1.1");

	if (request._method == "POST")
	{
		addEnvStuff("CONTENT_LENGTH", std::to_string(request.getBody()));

		auto content_type = request.getHeader("Content-Type");
		if (!content_type.empty())
			addEnvStuff("CONTENT_TYPE", content_type)
	}
}

void RequestHandler::CGIEnv::addEnvStuff(const std::string &name, const std::string& value)
{
	std::string env_var = name + "=" + value;
	auto buffer = std::make_unique<char[]>(env_var.length() + 1);
	std::strcpy(buffer.get(), env_var.c_str());
	env_array.push_back(buffer.get());
	env_bufs.push_back(std::move(buffer));
}