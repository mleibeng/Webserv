/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:27 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 16:05:51 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <string>
#include <map>



class HttpMessage
{
	public:
		HttpMessage();
		HttpMessage(const HttpMessage &other);
		HttpMessage& operator=(const HttpMessage &other);
		virtual ~HttpMessage();

		//setters
		void	setHttpVersion(const std::string& vers);
		void	setHeader(const std::string& key, const std::string& val);
		void	setBody(const std::string& body);

		//getters
		std::string		getHttpVersion() const;
		std::string		getBody() const;
		std::string		getHeader(const std::string& key) const;
		const std::map<std::string, std::string>&	getAllHeaders() const;

	protected:
		std::string							_httpVersion;
		std::string							_body;
		std::map<std::string, std::string>	_header;


};

#endif // HTTPMESSAGE_H
