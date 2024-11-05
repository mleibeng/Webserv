/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvinleibenguth <marvinleibenguth@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/06 00:49:50 by marvinleibe      ###   ########.fr       */
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

	response.setStatus(*route_conf.redirect_code);
	response.setHeader("Location", redirect);
	response.setBody("Redirecting to " + redirect);
	client.send_response(response.buildResponse());
}

/// @brief Entrypoint and management function for sorting Requests.
///		   Finds the most appropriate route and delegates the response to the specific type of request handler.
/// @param client client including the fd and request. Ultimately receives requests and sends the response.
void		RequestHandler::handleRequest(Client& client)
{
	const ServerConf* server_conf = findServerConf(client.getRequest());
	if (!server_conf)
		return serveErrorPage(client, 404);

	const RouteConf* route_conf = findRouteConf(*server_conf, client.getRequest());
	if (!route_conf)
		return serveErrorPage(client, 404);

	if (route_conf->redirect.has_value())
	{
		if (client.getNumRedirects() >= route_conf->max_redirects)
		{
			// std::cout << "num of redirects: "<< client.getNumRedirects() << std::endl;
			return serveErrorPage(client, 508);
		}
		// std::cout << "num of redirects: "<< client.getNumRedirects() << std::endl;
		return handleRedirect(*route_conf, client);
	}
	const std::string& method = client.getRequest().getMethod();

	if (!isMethodAllowed(*route_conf, method))
		return serveErrorPage(client, 405);

	std::string parsed = parsePath(*route_conf, client.getRequest());

	// std::cout << "Phy path: " << parsed << std::endl;

	if (method == "GET")
		handleGetRequest(client, *route_conf, parsed);
	else if (method == "POST")
		handlePostRequest(client, *route_conf, parsed);
	else if (method == "DELETE")
		handleDeleteRequest(client, *route_conf, parsed);
	else
		serveErrorPage(client, 501);
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
