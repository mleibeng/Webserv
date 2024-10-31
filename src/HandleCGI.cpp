/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:10:46 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 00:53:08 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/// @brief sets up pipes. Serve error page 500 on fail
/// @return ok : not ok
bool RequestHandler::setupPipes(PipeDescriptors &pipes, Client& client)
{
	if (pipe(pipes.in_pipe) == -1 || pipe (pipes.out_pipe) == -1)
	{
		serveErrorPage(client, 500);
		return false;
	}
	return true;
}

/// @brief close all pipes
void RequestHandler::PipeDescriptors::closeAll()
{
	close(in_pipe[0]);
	close(in_pipe[1]);
	close(out_pipe[0]);
	close(out_pipe[1]);
}

/// @brief close parent side pipes
void RequestHandler::PipeDescriptors::closeParentPipes()
{
	close(in_pipe[0]);
	close(out_pipe[1]);
}

/// @brief close child side pipes
void RequestHandler::PipeDescriptors::closeChildPipes()
{
	close(in_pipe[1]);
	close(out_pipe[0]);
}

/// @brief Central CGI handling function setting up pipes / forks for execution
/// @param client Client holds request and response functionality
/// @param cgi_path path to file that needs cgi execution
/// @param query CGI modification sent through the URI
void RequestHandler::handleCGI(Client& client, const std::string& cgi_path, const std::string& query)
{
	std::string file_extension = getFileExtension(cgi_path);
	try
	{
		auto handler = CGIHandleCreator::createHandler(file_extension);

		PipeDescriptors pipes;
		if (!setupPipes(pipes, client))
			return;

		const HttpRequest& request = client.getRequest();

		pid_t pid = fork();
		if (pid == -1)
		{
			pipes.closeAll();
			serveErrorPage(client, 500);
			return;
		}

		if (pid == 0)
			handleCGIChild(pipes, cgi_path, request, *handler);
		else
			handleCGIParent(pipes, client, request);
	}
	catch (const std::exception& e)
	{
		serveErrorPage(client, 500);
	}
}