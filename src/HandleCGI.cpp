/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:10:46 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 03:23:44 by mleibeng         ###   ########.fr       */
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
/// @param query CGI modification sent through the URI -> moved to HTTPRequest included in Client
void RequestHandler::handleCGI(Client& client, const std::string& cgi_path)
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

void RequestHandler::handleCGIChild(PipeDescriptors& pipes, const std::string& cgi_path, const HttpRequest& request, const CGIHandler& handler)
{
	pipes.closeChildPipes();

	dup2(pipes.in_pipe[0], STDIN_FILENO);
	dup2(pipes.out_pipe[1], STDOUT_FILENO);

	CGIEnv env(request, cgi_path, handler);

	std::vector<std::string> args_str = {handler.getCGIName(), cgi_path};
	auto extra_args = handler.getArgList();
	args_str.insert(args_str.begin() + 1, extra_args.begin(), extra_args.end());

	std::vector<char*> argv;
	for (const auto& arg : args_str)
		argv.push_back(strdup(arg.c_str()));

	argv.push_back(nullptr);
	execve(handler.getCGIPath().c_str(), argv.data(), env.getEnv());

	std::perror("execve error");
	for (char *arg : argv)
		free(arg);
	std::exit(1);
}

void RequestHandler::writeCGIInput(int pipe_fd, const std::string& request_body)
{
	write(pipe_fd, request_body.c_str(), request_body.length());
}

std::string RequestHandler::readCGIOutput(int pipe_fd)
{
	std::string output;
	char buffer[1024];
	ssize_t nbytes;
	while ((nbytes = read(pipe_fd, buffer, sizeof(buffer))) > 0)
		output.append(buffer, nbytes);

	if (nbytes == -1)
		throw std::runtime_error("Error in reading CGI Output");
	return output;
}

void RequestHandler::buildCGIResponse(const std::string& out, HttpResponse& response)
{
	std::istringstream out_stream(out);
	std::string line;
	bool headers_done = false;

	response.setStatus(200);

	while (std::getline(out_stream, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (line.empty())
		{
			headers_done = true;
			break;
		}
	}

	size_t cl_pos = line.find(":");
	if (cl_pos != std::string::npos)
	{
		std::string header_name = line.substr(0, cl_pos);
		std::string header_value = line.substr(cl_pos + 1);

		header_value = response.trimStr(header_value);

		if (header_name == "Status")
		{
			size_t s_pos = header_value.find(' ');
			if (s_pos != std::string::npos)
			{
				int status = std::stoi(header_value.substr(0, s_pos));
				response.setStatus(status);
			}
		}
		else
			response.setHeader(header_name, header_value);
	}
	std::stringstream body;
	if (headers_done)
		body << out_stream.rdbuf();
	else
	{
		out_stream.clear();
		out_stream.seekg(0);
		body << out_stream.rdbuf();
	}
	response.setBody(body.str());
	if (response.getHeader("Content-Type").empty() && !response.getBody().empty())
		response.setMimeType(".txt");
}

void RequestHandler::handleCGIParent(PipeDescriptors& pipes, Client& client, const HttpRequest& request)
{
	pipes.closeParentPipes();

	if (request.getMethod() == "POST")
		writeCGIInput(pipes.in_pipe[1], request.getBody());
	close(pipes.in_pipe[1]);

	std::string output = readCGIOutput(pipes.out_pipe[0]);
	close(pipes.out_pipe[0]);

	int status;
	waitpid(-1, &status, 0);

	HttpResponse response;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		buildCGIResponse(output, response);
	else
	{
		serveErrorPage(client, 500);
		return;
	}

	client.send_response(response.buildResponse());
}