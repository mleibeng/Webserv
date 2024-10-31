/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGICreator.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 23:07:04 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 00:12:11 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGICreator.hpp"

std::string PHPHandler::getCGIPath() const {return "/usr/bin/php-cgi";}
std::string PHPHandler::getCGIName() const {return "php-cgi";}
std::string PHPHandler::getDefaultContent() const {return "application/x-httpd-php";}

std::string PythonHandler::getCGIPath() const  {return "/usr/bin/python3";}
std::string PythonHandler::getCGIName() const  {return "python3";}
std::vector<std::string> PythonHandler::getArgList() const {return {"-u"};}
std::string PythonHandler::getDefaultContent() const  {return "text/x-python";}

std::unique_ptr<CGIHandler> CGIHandleCreator::createHandler(const RouteConf& route_conf, const std::string& file_extension)
{
	if (!route_conf.cgi_extension.empty() && route_conf.cgi_extension == file_extension)
	{
		if (file_extension == ".php")
			return std::make_unique<PHPHandler>();
		if (file_extension == ".py")
			return std::make_unique<PythonHandler>();
	}
	throw std::runtime_error("Unsupported File extension for CGI Handling detected");
}