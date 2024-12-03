/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:50 by fwahl             #+#    #+#             */
/*   Updated: 2024/12/03 21:08:49 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : AHttpMessage() {}

HttpResponse::HttpResponse(const HttpResponse &other) : AHttpMessage()
{
	*this = other;
}

HttpResponse& HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
		AHttpMessage::operator=(other);
		setStatus(other.getStatus());
		setMimeType(other._mimeType);
	}
	return *this;
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setStatus(int status)
{
	_status = HttpResponse::itos(status);
}

void HttpResponse::setStatus(StatusCode status)
{
	_status = status;
}

void HttpResponse::setMimeType(std::string extension)
{
	this->setHeader("Content-Type", getMimeType(extension));
}

int HttpResponse::getStatus() const
{
	return HttpResponse::stoi();
}

std::string HttpResponse::getMimeType(const std::string extension)
{
	static const std::map<std::string, std::string> mimeTypes =
	{
		{".html", "text/html"},
		{".htm", "text/html"},
		{".css", "text/css"},
		{".js", "application/javascript"},
		{".json", "application/json"},
		{".xml", "application/xml"},
		{".png", "image/png"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".gif", "image/gif"},
		{".bmp", "image/bmp"},
		{".ico", "image/x-icon"},
		{".svg", "image/svg+xml"},
		{".txt", "text/plain"},
		{".pdf", "application/pdf"},
		{".zip", "application/zip"},
		{".tar", "application/x-tar"},
		{".mp3", "audio/mpeg"},
		{".mp4", "video/mp4"},
		{".php", "application/x-httpd-php"}
		// application/octet-stream ->download files
	};
	auto	iter = mimeTypes.find(extension);
	if (iter == mimeTypes.end())
		return (""); // do this later
	return (iter->second);
}

bool HttpResponse::parse(const std::string& rawmsg)
{
	std::istringstream	input(rawmsg);
	std::string			line;

	if (!std::getline(input, line))
		return (false);

	std::istringstream	status(line);
	std::string			vers;
	std::string			status_code;
	std::string			status_txt;

	if (!(status >> vers >> status_code >> std::ws))
		return (false);

	std::getline(status, status_txt);
	setHttpVersion(vers);
	setStatus(std::stoi(status_code));
	parseHeader(input);

	std::string	body;
	std::string	cont_len_str = getHeader("Content-Length");

	if (!cont_len_str.empty())
	{
		size_t	cont_len = std::stoul(cont_len_str);
		std::vector<char>	body_buff(cont_len);
		input.read(body_buff.data(), cont_len);
		body.assign(body_buff.begin(), body_buff.end());
	}
	else
	{
		std::stringstream	body_stream;
		body_stream << input.rdbuf();
		body = body_stream.str();
	}
	setBody(body);

	return (true);
}

void HttpResponse::removeCookie(const std::string& key, const std::string& path)
{
	setCookie(key, "", 0, path);
}

void HttpResponse::setCookie(const std::string& key, const std::string& value, int maxAge, const std::string &path, bool secure, bool HttpOnly)
{
	std::ostringstream cookie;
	cookie << key << "=" << value;
	if (!path.empty())
		cookie << "; Path=" << path;
	if (maxAge >= 0)
		cookie << "; Max-Age=" << maxAge;
	if (secure)
		cookie << "; Secure";
	if (HttpOnly)
		cookie << "; HttpOnly";

	_response_cookies.push_back(cookie.str());
}

std::string HttpResponse::buildResponse() const
{
	std::ostringstream	response;

	response << "HTTP/1.1 " << HttpResponse::stoi() << " " << HttpResponse::stostr() << "\r\n";

	// std::cout << RED << this->getHeader("Content-Type") << RESET << std::endl;
	for (const auto& header : _header)
	{
		response << header.first << ": " << header.second << "\r\n";
		// std::cout << header.first << std::endl;
		// std::cout << header.second << std::endl;
	}
	for (const auto& cookie: _response_cookies)
	{
		response << "Set-Cookie: " << cookie << "\r\n";
	}

	if (_header.find("Content-Length") == _header.end())
		response << "Content-Length: " << _body.size() << "\r\n";
	if (_header.find("Content-Type") == _header.end())
		response << "Content-Type: text/html\r\n";
	response << "\r\n";
	response << _body;

	return (response.str());
}

std::string HttpResponse::stostr() const
{
	switch(_status)
	{
		case StatusCode::OK:
			return ("OK");
		case StatusCode::CREATED:
			return ("Created");
		case StatusCode::NOCONTENT:
			return ("No Content");
		case StatusCode::MOVED_PERM:
			return ("Moved Permanently");
		case StatusCode::MOVED_TEMP:
			return ("Moved temporarily");
		case StatusCode::NOT_MODIFIED:
			return ("Not Modified");
		case StatusCode::BAD_REQUEST:
			return ("Bad Request");
		case StatusCode::FORBIDDEN:
			return ("Forbidden");
		case StatusCode::NOT_FOUND:
			return ("Not Found");
		case StatusCode::METH_NOT_ALLOWED:
			return ("Method Not Allowed");
		case StatusCode::CONFLICT:
			return ("Conflict");
		case StatusCode::INTERNAL_SERV_ERR:
			return ("Internal Server Error");
		case StatusCode::NOT_IMPLEMENTED:
			return ("Not implemented");
		case StatusCode::ENDLESS_LOOP:
			return ("Redirection Loop");
		default:
			return ("Unknown Status");
	}
}

StatusCode HttpResponse::itos(int code) const
{
	return static_cast<StatusCode>(code);
}

int HttpResponse::stoi() const
{
	return static_cast<int>(_status);
}
