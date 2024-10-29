/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:10:46 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/30 00:35:02 by mleibeng         ###   ########.fr       */
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

		int status;
		waitpid(pid, &status, 0);

		HttpResponse response;
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			parseCGIOutput(cgi_output, response);
		else
			serveErrorPage(client, 500);
		client.send_response(response.buildResponse());
	}
}

char** RequestHandler::CGIEnvironment::getEnvpArray()
{
	return env_array.data();
}

void RequestHandler::CGIEnvironment::setupEnvironment(const std::string& query, const std::string& cgi_path)
{
		env_array.clear();

		// Initialize environment variables with proper null-termination
		std::snprintf(req_method, BUFFER_SIZE, "REQUEST_METHOD=GET");
		std::snprintf(query_string, BUFFER_SIZE, "QUERY_STRING=%s", query.c_str());
		std::snprintf(content_length, BUFFER_SIZE, "CONTENT_LENGTH=0");
		std::snprintf(script_filename, BUFFER_SIZE, "SCRIPT_FILENAME=%s", cgi_path.c_str());
		std::snprintf(redirect_status, BUFFER_SIZE, "REDIRECT_STATUS=200");
		std::snprintf(content_type, BUFFER_SIZE, "CONTENT_TYPE=application/x-httpd-php");

		// Add pointers to env_array
		env_array.push_back(req_method);
		env_array.push_back(query_string);
		env_array.push_back(content_length);
		env_array.push_back(script_filename);
		env_array.push_back(redirect_status);
		env_array.push_back(content_type);
		env_array.push_back(nullptr);
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
	std::strncpy(script_path, cgi_path.c_str(), sizeof(script_path) - 1);
	script_path[sizeof(script_path) - 1] = '\0';

	char* argv[] =
	{
		php_name,
		script_path,
		NULL
	};

	execve(php_path, argv, env.getEnvpArray());
	std::perror("execve");
	std::exit(1);
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

void RequestHandler::parseCGIOutput(const std::string& cgi_output, HttpResponse& response)
{
	std::istringstream iss(cgi_output);
	std::string line;
	bool headers_done = false;
	std::string body;
	std::stringstream body_stream;

	// Parse headers
	while (std::getline(iss, line) && !headers_done)
	{
		// Check for empty line or just \r that indicates end of headers
		if (line.empty() || line == "\r")
		{
			headers_done = true;
			continue;
		}

		// Remove \r if present
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);

		// Parse header
		size_t colon_pos = line.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string header_name = line.substr(0, colon_pos);
			std::string header_value = line.substr(colon_pos + 1);

			// Trim leading/trailing whitespace from header value
			header_value.erase(0, header_value.find_first_not_of(" "));
			header_value.erase(header_value.find_last_not_of(" ") + 1);

			response.setHeader(header_name, header_value);
		}
	}

	// Read the rest of the body
	if (headers_done)
	{
		while (std::getline(iss, line))
		{
			if (line[line.length() - 1] == '\r')
				line.erase(line.length() - 1);
			body_stream << line << "\n";
		}
	}
	else
	{
		// If no headers were found, treat entire output as body
		iss.clear();
		iss.seekg(0);
		body_stream << iss.rdbuf();
	}

	body = body_stream.str();

	// Set response properties
	if (response.getHeader("Content-Type").empty()) {
		response.setHeader("Content-Type", "text/html");
	}

	// Ensure we have proper HTML structure if it's missing
	if (body.find("<html>") == std::string::npos) {
		std::string full_html = "<html><body>\n" + body;
		if (body.find("</html>") == std::string::npos) {
			full_html += "</body></html>";
		}
		body = full_html;
	}

	response.setStatus(200);  // Default success status
	response.setBody(body);
}


RequestHandler::CGIEnvironment::~CGIEnvironment()
{
	env_array.clear();
}