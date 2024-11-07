/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/07 11:26:22 by mleibeng         ###   ########.fr       */
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

	return true;
}

bool HttpRequest::parseMultipartBoundary()
{
	std::string content_type = getHeader("Content-Type");
	size_t boundary_pos = content_type.find("boundary=");
	if (boundary_pos == std::string::npos)
		return false;

	_boundary = "--" + trimStr(content_type.substr(boundary_pos + 9));
	_state = State::R_MULTIPART;
	return true;
}

void HttpRequest::handleMultipartChunk(const std::string& chunky)
{
	size_t boundary_pos = chunky.find(_boundary);
	if (boundary_pos == std::string::npos)
	{
		_body += chunky;
		return;
	}
	if (boundary_pos > 0)
		_body += chunky.substr(0, boundary_pos);

	size_t next_boundary = chunky.find(_boundary, boundary_pos + _boundary.length());
	if (next_boundary == std::string::npos)
	{
		_state = State::R_BODY;
		return;
	}
	std::string part = chunky.substr(boundary_pos + _boundary.length(), next_boundary - boundary_pos - _boundary.length());
	//handle later
}

bool	HttpRequest::hasHeader(const std::string& header) const
{
	if (_header.find(header) != _header.end())
		return true;
	return false;
}

std::string generateFilename()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	std::stringstream ss;
	ss << "upload_" << now_ms << "_" << std::rand();
	return ss.str();
}

bool HttpRequest::initUpload(const RouteConf& route) {
	if (!route.upload_dir.has_value())
		return false;

	std::string filename = generateFilename();
	_upload_path = route.path + "/" + filename;
	_upload_file = std::make_unique<std::ofstream>(filename, std::ios::binary);

	return _upload_file->is_open();
}

// bool	HttpRequest::parse(const std::string& rawmsg)
// {
// 	std::istringstream	input(rawmsg);
// 	std::string			line;

// 	if (!std::getline(input, line))
// 		return (false);

// 	parseHeader(input);

// 	std::string body((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
// 	setBody(trimStr(body));
// 	return (true);
// }
