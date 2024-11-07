/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:45 by fwahl             #+#    #+#             */
/*   Updated: 2024/11/07 03:42:30 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : _state(State::R_HEADER), _expected_content_len(0), _byte_read(0)
{}

HttpRequest::HttpRequest(const HttpRequest &other) : AHttpMessage()
{
	*this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest &other)
{
	if (this != &other)
	{
		AHttpMessage::operator=(other);
		setMethod(other.getMethod());
		_uri = other.getUri();
		setQuery(other.getQuery());
	}
	return (*this);
}

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

std::string generateFileName()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	std::stringstream ss;
	ss << "upload" << now_ms << "" << std::rand();
	return ss.str();
}

void	HttpRequest::initUpload(const RouteConf& route_conf)
{
	if (_upload_file)
		return;
	std::string filename = generateFileName();
	_upload_path = route_conf.upload_dir + "/" + filename;
	std::filesystem::create_directories(route_conf.upload_dir);
	_upload_file = std::make_unique<std::ofstream>(_upload_path, std::ios::binary);

	if (!_upload_file->is_open())
		throw std::runtime_error("Yo wtf is happening bro upload files are fked up!!");
}

bool	HttpRequest::parseHeaderChunk(const std::string& chunky)
{
	_part_headers += chunky;

	size_t header_end = _part_headers.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return true;

	std::string header_section = _part_headers.substr(0, header_end);
	if (!parse(header_section))
		return false;

	const std::string& content_length = getHeader("Content-Length");
	if(!content_length.empty())

	try
	{
		_expected_content_len = std::stoull(content_length);
	}
	catch (...)
	{
		return false;
	}

	_state = State::ROUTING;

	const std::string& content_type = getHeader("Content-Type");
	if (content_type.find("multipart/form-data") != std::string::npos)
	{
		size_t boundary_pos = content_type.find("boundary=");
		if (boundary_pos != std::string::npos)
			_boundary = content_type.substr(boundary_pos + 9);
	}

	if (header_end + 4 < _part_headers.size())
	{
		std::string body_chunk = _part_headers.substr(header_end + 4);
		_part_headers.clear();
		return parseBodyChunk(body_chunk);
	}
	_part_headers.clear();
	return true;
}

bool	HttpRequest::parseBodyChunk(const std::string& chunky)
{
	if (_state != State::R_BODY)
		return false;

	if (_upload_file)
	{
		_upload_file->write(chunky.c_str(), chunky.size());
		if (!_upload_file->good())
			return false;
	}
	else
		_body += chunky;

	_byte_read += chunky.size();
	if (_byte_read >= _expected_content_len)
	{
		_state = State::COMPLETE;
		if (_upload_file)
			_upload_file->close();
	}
	return true;
}

bool	HttpRequest::parse(const std::string& rawmsg)
{
	std::istringstream	input(rawmsg);
	std::string			line;

	if (!std::getline(input, line))
		return (false);

	std::istringstream	reqLine(line);
	std::string	method;
	std::string	vers;
	std::string	uri;

	if (!(reqLine >> method >> uri >> vers))
		return (false);

	setMethod(trimStr(method));
	setQuery(trimStr(uri)); // now splits query from uri and sets both.
	setHttpVersion(trimStr(vers));

	if (!parseHeader(input))
		return false;

	return (true);
}
