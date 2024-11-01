/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnv.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 00:55:12 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 02:14:35 by mleibeng         ###   ########.fr       */
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
	std::string method = request.getMethod();

	addEnvStuff("REQUEST_METHOD", method);
	addEnvStuff("QUERY_STRING", request.getQuery());
	addEnvStuff("SCRIPT_FILE", cgi_path);
	addEnvStuff("REDIRECT_STATUS", "200");
	addEnvStuff("PATH_INFO", "");
	addEnvStuff("SERVER_SOFTWARE", "WebServer/1.1");

	if (method == "POST")
	{
		addEnvStuff("CONTENT_LENGTH", std::to_string(request.getBody().length()));

		auto content_type = request.getHeader("Content-Type");
		if (!content_type.empty())
			addEnvStuff("CONTENT_TYPE", content_type);
		else
			addEnvStuff("CONTENT_TYPE", "application/x-www-form-urlencoded");
	}
	else
	{
		addEnvStuff("CONTENT_LENGTH", "0");
		addEnvStuff("CONTENT_TYPE", handler.getDefaultContent());
	}

	const auto& headers = request.getAllHeaders();
	for (const auto& [header, value] : headers)
	{
		std::string env_name = "HTTP_ " + header;
		std::transform(env_name.begin(), env_name.end(), env_name.begin(), ::toupper);
		std::replace(env_name.begin(), env_name.end(), '-', '_');
		addEnvStuff(env_name, value);
	}

	env_array.push_back(nullptr);
}

void RequestHandler::CGIEnv::addEnvStuff(const std::string &name, const std::string& value)
{
	std::string env_var = name + "=" + value;
	auto buffer = std::make_unique<char[]>(env_var.length() + 1);
	std::strcpy(buffer.get(), env_var.c_str());
	env_array.push_back(buffer.get());
	env_bufs.push_back(std::move(buffer));
}