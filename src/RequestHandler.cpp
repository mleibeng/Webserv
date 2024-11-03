/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/03 20:09:51 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

void RequestHandler::handleRedirect(const RouteConf& route_conf, Client& client)
{
	HttpResponse response;

	response.setStatus(*route_conf.redirect_code);
	response.setHeader("Location", *route_conf.redirect);
	response.setBody("Redirecting to " + *route_conf.redirect);
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
		return handleRedirect(*route_conf, client);

	const std::string& method = client.getRequest().getMethod();

	if (!isMethodAllowed(*route_conf, method))
		return serveErrorPage(client, 405);

	std::string parsed = parsePath(*route_conf, client.getRequest());

	std::cout << "Phy path: " << parsed << std::endl;

	if (method == "GET")
		handleGetRequest(client, *route_conf, parsed);
	else if (method == "POST")
		handlePostRequest(client, *route_conf, parsed);
	else if (method == "DELETE")
		handleDeleteRequest(client, *route_conf, parsed);
	else
		serveErrorPage(client, 501);
}

// und hier das argument nr2 so: ->  handleGetRequest(request, const RouteConf &route_conf)!
void		RequestHandler::handlePostRequest(Client& client, const RouteConf& route_conf, const std::string& parsed)
{
	// std::string file_path = route_conf.root + request.getUri();
	(void)client;
	(void)route_conf;
	(void)parsed;
	// HttpResponse response;

	// DIESE LOGIK MUSS REIN <- filedescriptor koennte ein issue sein. weil CGI schickt selber zurueck und baut keine Nachricht!!
	// if (!route_conf.cgi_extension.empty() && request.getUri().ends_with(route_conf.cgi_extension))
	//		handleCGI(int fd, file_path, request.getBody());
	// else
	//		normale handler logik!
	// client.send_response(response.buildResponse());
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
