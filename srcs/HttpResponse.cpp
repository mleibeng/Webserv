/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:50 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/08 02:31:05 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	setStatus(StatusCode::OK);
}

HttpResponse::HttpResponse(const HttpResponse &other)
{
	*this = other;
}

HttpResponse& HttpResponse::operator=(const HttpResponse &other)
{
	if (this != &other)
	{
		AHttpMessage::operator=(other);
		setStatus(other.getStatus());
	}
	return (*this);
}

HttpResponse::~HttpResponse()
{
}

//SETTERS

void HttpResponse::setStatus(StatusCode status)
{
	_status = status;
	// change Status to input
	// and make check if it is even acceptable status as well?
}

//GETTERS

StatusCode	HttpResponse::getStatus() const
{
	return (_status);
}

bool	HttpResponse::parse(const std::string& rawmsg)
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
	int	code = std::stoi(status_code);
	setStatus(static_cast<StatusCode>(code));
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
