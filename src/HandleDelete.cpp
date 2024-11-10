/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleDelete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:57:22 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/10 02:58:26 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

void	RequestHandler::handleDeleteRequest(Client& client)
{
	const HttpRequest&	request = client.getRequest();
	const RouteConf*	route_conf = client.getRoute();

	//eigentlich schon pre checked
	if (!client.isMethodAllowed(*route_conf, "DELETE"))
		return (serveErrorPage(client, 405));

	//probier hier anstatt file_path mal die getBestPath function aus, sollte klappen
	// std::string _filepath = client.getBestPath();
	std::string			uri_path = request.getUri();
	std::string			filename = uri_path.substr(uri_path.find_last_of('/') + 1);
	std::filesystem::path	upload_dir = std::filesystem::current_path() / "html_pages/uploads";
	std::filesystem::path	file_path = upload_dir / filename;

	if (!std::filesystem::exists(file_path))
		return (serveErrorPage(client, 404));
	if (!std::filesystem::equivalent(file_path.parent_path(), upload_dir))
		return (serveErrorPage(client, 403));
	if (std::filesystem::remove(file_path))
	{
			HttpResponse response;
			response.setStatus(204);
			response.setBody(filename + " successfully deleted");
			response.setMimeType(".html");
			client.send_response(response.buildResponse());
	}
	else
	{
		return (serveErrorPage(client, 500));
	}
}
