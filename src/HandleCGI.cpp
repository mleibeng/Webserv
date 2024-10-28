/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:10:46 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/28 22:05:03 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

bool RequestHandler::setupPipes(PipeDescriptors& pipes, Client& client)
{
	if (pipe(pipes.in_pipe) == -1 || pipe(pipes.out_pipe) == -1)
	{
		perror("pipe error");
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

void RequestHandler::handleCGI(Client& client, const std::string& cgi_path, const std::string& query)
{
	PipeDescriptors pipes;
	if (!setupPipes(pipes, client))
		return;

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		pipes.closeAll();
		serveErrorPage(client, 500);
		return;
	}

	if (pid == 0)
	{
		CGIEnvironment env;
		env.setupEnvironment(query, cgi_path);
		handleChildProc(pipes, cgi_path, env);
	}
	else
	{
		close(pipes.out_pipe[1]);
		close(pipes.in_pipe[0]);

		std::string cgi_output = readCGIOutput(pipes.out_pipe[0]);
		close(pipes.out_pipe[0]);
		waitpid(pid, NULL, 0);

		std::string body = extractRespBody(cgi_output);
		sendResponse(client, body);
	}
}

char** RequestHandler::CGIEnvironment::getEnvpArray()
{
	static char* envp[] =
	{
		req_method,
		query_string,
		content_length,
		script_filename,
		redirect_status,
		NULL
	};
	return envp;
}

void RequestHandler::CGIEnvironment::setupEnvironment(const std::string& query, const std::string& cgi_path)
{
	std::strcpy(req_method, "REQUEST_METHOD=GET");
	std::strcpy(query_string, "QUERY_STRING=");
	std::strcpy(content_length, "CONTENT_LENGTH=0");
	std::strcpy(script_filename, "SCRIPT_FILENAME=");
	std::strcpy(redirect_status, "REDIRECT_STATUS=200");

	std::strncat(query_string, query.c_str(), sizeof(query_string) - strlen(query_string) - 1);
	std::strncat(script_filename, cgi_path.c_str(), sizeof(script_filename) - strlen(script_filename) - 1);
}

void RequestHandler::handleChildProc(const PipeDescriptors& pipes, const std::string& cgi_path, CGIEnvironment& env)
{
	close(pipes.out_pipe[0]);
	close(pipes.in_pipe[1]);
	dup2(pipes.in_pipe[0], STDIN_FILENO);
	dup2(pipes.out_pipe[1], STDOUT_FILENO);

	char php_path[] = "/usr/bin/php-cgi";
	char php_name[] = "php-cgi";
	char script_path[4096];
	strncpy(script_path, cgi_path.c_str(), sizeof(script_path) - 1);
	script_path[sizeof(script_path) - 1] = '\0';

	char* argv[] =
	{
		php_name,
		script_path,
		NULL
	};

	execve(php_path, argv, env.getEnvpArray());
	perror("execve");
	exit(1);
}

std::string RequestHandler::readCGIOutput(int pipe_fd)
{
	std::string cgi_output;
	char buffer[4096];
	int bytes_read;

	while ((bytes_read = read(pipe_fd, buffer, sizeof(buffer))) > 0)
		cgi_output.append(buffer, bytes_read);

	return cgi_output;
}

std::string RequestHandler::extractRespBody(const std::string& cgi_output)
{
	size_t header_end = cgi_output.find("\r\n\r\n");
	if (header_end == std::string::npos)
		header_end = cgi_output.find("\n\n");

	if (header_end != std::string::npos)
		return cgi_output.substr(header_end + (cgi_output[header_end + 1] == '\n' ? 2 : 4));

	return cgi_output;
}

void RequestHandler::sendResponse(Client& client, const std::string& body)
{
	HttpResponse response;
	response.setStatus(200);
	response.setBody(body);
	response.setMimeType("text/html");
	client.send_response(response.buildResponse());
}