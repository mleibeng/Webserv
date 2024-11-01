/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnv.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 00:55:12 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/01 01:02:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::CGIEnv::CGIEnv(const HttpRequest& request, const std::string& cgi_path, const CGIHandler& handler)
{

}

RequestHandler::CGIEnv:~CGIEnv() {}

char** RequestHandler::CGIEnv::getEnv()
{

}