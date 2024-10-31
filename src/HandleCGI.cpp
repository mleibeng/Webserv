/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:10:46 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 00:37:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

bool RequestHandler::setupPipes(PipeDescriptors &pipes, Client& client)
{
	if (pipe(pipes.in_pipe) == -1 || pipe (pipes.out_pipe) == -1)
	{
		serveErrorPage(client, 500);
		return false;
	}
	return true;
}

void RequestHandler::PipeDescriptors::closeAll()
{
	close(in_pipe[0]);
	close(in_pipe[1]);
	close(out_pipe[0]);
	close(out_pipe[1]);
}

void RequestHandler::PipeDescriptors::closeParentPipes()
{
	close(in_pipe[0]);
	close(out_pipe[1]);
}

void RequestHandler::PipeDescriptors::closeChildPipes()
{
	close(in_pipe[1]);
	close(out_pipe[0]);
}
