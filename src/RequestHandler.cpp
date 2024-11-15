/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/15 02:48:52 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

std::string RequestHandler::buildRedirWQuery(const RouteConf& route_conf, const HttpRequest& request)
{
	std::string redirect_with_query = *route_conf.redirect;

	if (!request.getQuery().empty())
	{
		if (redirect_with_query.find('?') == std::string::npos)
			redirect_with_query += "?" + request.getQuery();
		else
			redirect_with_query += "&" + request.getQuery();
	}

	return redirect_with_query;
}

void RequestHandler::handleRedirect(const RouteConf& route_conf, Client& client)
{
	HttpResponse response;

	client.increaseRedirectCount();

	std::string redirect = buildRedirWQuery(route_conf, client.getRequest());

	response.setStatus(route_conf.redirect_code.value_or(302));
	response.setHeader("Location", redirect);
	response.setBody("Redirecting to " + redirect);
	client.send_response(response.buildResponse());
}

bool RequestHandler::resolveRouting(Client& client)
{

	const std::string& method = client.getRequest().getMethod();

	if (method == "GET")
		handleGetRequest(client);
	else if (method == "POST")
		handlePostRequest(client);
	else if (method == "DELETE")
		handleDeleteRequest(client);
	else
		serveErrorPage(client, 501);
}

// UTILS

std::string		getFileExtension(const std::string& filepath)
{
	size_t dot_pos = filepath.find_last_of(".");
		if (dot_pos != std::string::npos)
			return filepath.substr(dot_pos);
		return "";
}
