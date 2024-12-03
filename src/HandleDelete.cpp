/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleDelete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:57:22 by fwahl             #+#    #+#             */
/*   Updated: 2024/12/04 00:37:11 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

void	RequestHandler::handleDeleteRequest(Client& client)
{
	//probier hier anstatt file_path mal die getBestPath function aus, sollte klappen
	// std::string			filename = uri_path.substr(uri_path.find_last_of('/') + 1);
	std::filesystem::path	file_path = client.getBestPath();

	if (!std::filesystem::exists(file_path))
		return (serveErrorPage(client, 404));
	// if (!std::filesystem::equivalent(file_path.parent_path(), upload_dir))
	// 	return (serveErrorPage(client, 403));
	if (std::filesystem::remove(file_path))
	{
			HttpResponse response;
			response.setStatus(204);
			response.setBody("successfully deleted");
			response.setMimeType(".html");
			client.setResponseString(response.buildResponse());
			// client.send_response(response.buildResponse());
	}
	else
	{
		return (serveErrorPage(client, 500));
	}
}
