/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/15 02:48:46 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{}

// HttpRequest::HttpRequest(const HttpRequest &other) : AHttpMessage()
// {
// 	*this = other;
// }

// HttpRequest& HttpRequest::operator=(const HttpRequest &other)
// {
// 	if (this != &other)
// 	{
// 		AHttpMessage::operator=(other);
// 		setMethod(other.getMethod());
// 		_uri = other.getUri();
// 		setQuery(other.getQuery());
// 	}
// 	return (*this);
// }

HttpRequest::~HttpRequest()
{
}

const std::string& HttpRequest::getMethod() const
{
	return (_method);
}

const std::string&	HttpRequest::getUri() const
{
	return (_uri);
}

const std::string& HttpRequest::getQuery() const
{
	return (_query);
}

void	HttpRequest::setMethod(const std::string& method)
{
	_method = method;
}

void	HttpRequest::setQuery(const std::string& uri)
{
	size_t query_point = uri.find('?');
	if (query_point != std::string::npos)
	{
		_query = uri.substr(query_point + 1);
		_uri = uri.substr(0, query_point);
	}
	else
	{
		_query.clear();
		_uri = uri;
	}
}

bool HttpRequest::parseHeaderChunk(const std::string& chunk) {
	_part_headers += chunk;

	// Look for end of headers
	size_t header_end = _part_headers.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return true; // Need more data

	std::string headers = _part_headers.substr(0, header_end);
	std::istringstream stream(headers);
	std::string line;

	// Parse request line
	if (!std::getline(stream, line) || !parseRequestLine(line))
		return false;

	// Parse headers
	while (std::getline(stream, line) && !line.empty()) {
		if (line.back() == '\r')
			line.pop_back();

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;

		std::string key = trimStr(line.substr(0, colon));
		std::string value = trimStr(line.substr(colon + 1));
		setHeader(key, value);
	}

	// Check for Content-Length and Content-Type
	if (("Content-Length"))
		_expected_content_len = std::stoull(getHeader("Content-Length"));

	if (hasHeader("Content-Type"))
	{
		std::string content_type = getHeader("Content-Type");
		if (content_type.find("multipart/form-data") != std::string::npos)
			parseMultipartBoundary();
	}

	// Handle remaining data as body
	_part_headers = _part_headers.substr(header_end + 4);
	_state = State::ROUTING;

	return true;
}

bool HttpRequest::parseBodyChunk(const std::string& chunk) {
	if (_state == State::R_MULTIPART) {
		handleMultipartChunk(chunk);
	} else {
		_byte_read += chunk.length();
		if (_upload_file) {
			_upload_file->write(chunk.c_str(), chunk.length());
		} else {
			_body += chunk;
		}
	}

	if (_byte_read >= _expected_content_len) {
		_state = State::COMPLETE;
		if (_upload_file)
			_upload_file->close();
	}

	return true;
}

bool HttpRequest::parseRequestLine(const std::string& line)
{
	std::istringstream	reqLine(line);
	std::string	method;
	std::string	vers;
	std::string	uri;

	if (!(reqLine >> method >> uri >> vers))
		return (false);

	setMethod(trimStr(method));
	setQuery(trimStr(uri)); // now splits query from uri and sets both.
	setHttpVersion(trimStr(vers));

	parseHeader(input);

	std::string body((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	setBody(trimStr(body));

	parseCookies();
	return (true);
}
