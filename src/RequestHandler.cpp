/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/07 06:06:43 by mleibeng         ###   ########.fr       */
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

void		RequestHandler::handleDeleteRequest(Client& client)
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
