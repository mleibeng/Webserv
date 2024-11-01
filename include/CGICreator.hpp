/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGICreator.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:52:24 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 01:38:35 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGICREATOR_HPP
#define CGICREATOR_HPP

#include <vector>
#include <string>

//made the handler code more modular
class CGIHandler
{
	public:
	virtual ~CGIHandler() = default;
	virtual std::string getCGIPath() const = 0;
	virtual std::string getCGIName() const = 0;
	virtual std::vector<std::string> getArgList() const {return {};}
	virtual std::string getDefaultContent() const = 0;
};

class PHPHandler : public CGIHandler
{
	std::string getCGIPath() const override;
	std::string getCGIName() const override;
	std::string getDefaultContent() const override;
};


class PythonHandler : public CGIHandler
{
	std::string getCGIPath() const override;
	std::string getCGIName() const override;
	std::vector<std::string> getArgList() const override;
	std::string getDefaultContent() const override;
};

//class perlhandler

//class rubyhandler

//class etc...

class CGIHandleCreator
{
	public:
	static std::unique_ptr<CGIHandler> createHandler(const std::string &file_extension);
};

#endif