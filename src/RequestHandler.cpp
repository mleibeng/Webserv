/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/07 03:12:54 by mleibeng         ###   ########.fr       */
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
	const ServerConf* server_conf = findServerConf(client.getRequest());
		if (!server_conf)
			return serveErrorPage(client, 404), false;

		const RouteConf* route_conf = findRouteConf(*server_conf, client.getRequest());
		if (!route_conf)
			return serveErrorPage(client, 404), false;

		// Handle redirects
		if (route_conf->redirect.has_value()) {
			if (client.getNumRedirects() >= route_conf->max_redirects)
				return serveErrorPage(client, 508), false;
			return handleRedirect(*route_conf, client), false;
		}

		// Validate method
		const std::string& method = client.getRequest().getMethod();
		if (!isMethodAllowed(*route_conf, method))
			return serveErrorPage(client, 405), false;

		// Set route and transition to body reading if needed
		client.setRoute(route_conf);
		return true;
}


void RequestHandler::processCompleteRequest(Client& client)
{
	const std::string& method = client.getRequest().getMethod();
	const RouteConf* route = client.getRoute();

	if (!route)
		return serveErrorPage(client, 404);

	std::string parsed = parsePath(*route, client.getRequest());

	if (method == "GET")
		handleGetRequest(client, *route, parsed);
	else if (method == "POST")
		handlePostRequest(client, *route, parsed);
	else if (method == "DELETE")
		handleDeleteRequest(client, *route, parsed);
	else
		serveErrorPage(client, 501);
}

/// @brief Entrypoint and management function for sorting Requests.
///		   Finds the most appropriate route and delegates the response to the specific type of request handler.
/// @param client client including the fd and request. Ultimately receives requests and sends the response.
void	RequestHandler::handleRequest(Client& client)
{

	if (client.needsRouteResolution())
	{
		if (!resolveRouting(client))
			return;
	}

	switch (client.getRequest().getState())
	{
		case HttpRequest::State::R_BODY:
			break;
		case HttpRequest::State::COMPLETE:
			processCompleteRequest(client);
			break;
		case HttpRequest::State::ERROR:
			serveErrorPage(client, 400);
			break;
		default:
			break;
	}
}

void		RequestHandler::handleDeleteRequest(Client& client, const RouteConf& route_conf, const std::string& parsed)
{
	// std::string file_path = route_conf.root + request.getUri();

	// LOGIK:
	//  |
	//  V
	// if (std::filesystem::exists(file_path) && !std::filesystem::is_directory(file_path))
	// {
	// 	if (std::filesystem::remove(file_path))
	// 		client.send_response(HttpResponse(200, "text/plain", "File deleted successfully"));
	// 	else
	// 		serveErrorPage(client, 500); // Internal Server Error
	// }
	// else
	// 	serveErrorPage(client, 404); // Not

	(void)client;
	(void)route_conf;
	(void)parsed;
	// HttpResponse response;
	// client.send_response(response.buildResponse());
}

// UTILS

std::string		getFileExtension(const std::string& filepath)
{
	size_t dot_pos = filepath.find_last_of(".");
		if (dot_pos != std::string::npos)
			return filepath.substr(dot_pos);
		return "";
}
