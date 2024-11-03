/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:27 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/08 00:59:23 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTPMESSAGE_H
#define AHTTPMESSAGE_H

#define RED			"\033[31m"
#define YELLOW		"\033[33m"
#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>


class AHttpMessage
{
	public:
		AHttpMessage();
		AHttpMessage(const AHttpMessage &other);
		AHttpMessage& operator=(const AHttpMessage &other);
		virtual ~AHttpMessage();

		//setters
		void	setHttpVersion(const std::string& vers);
		void	setHeader(const std::string& key, const std::string& val);
		void	setAllHeaders(const std::map<std::string, std::string>& headers);
		void	setBody(const std::string& body);

		//getters
		std::string		getHttpVersion() const;
		std::string		getBody() const;
		std::string		getHeader(const std::string& key) const;
		const std::map<std::string, std::string>&	getAllHeaders() const;

		//parse
		void			parseHeader(std::istringstream& input);
		virtual bool	parse(const std::string& rawMessage) = 0;
		static std::string	trimStr(const std::string& str);

	protected:
		std::string							_httpVersion;
		std::string							_body;
		std::map<std::string, std::string>	_header;
};

#endif // AHTTPMESSAGE_H
