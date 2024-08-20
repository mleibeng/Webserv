/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:00 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 07:06:53 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Manage client connections
Handles: FileDescriptor Inheritance for socket I/O handling & Processing of incoming requests and responses.
*/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "FileDescriptor.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include <array>

class Connection : public FileDescriptor
{
	private:
	Request request;
	Response response;

	public:
	Connection();

	void handleRead() override;
	void handleWrite() override;
	void handleError() override;
};

#endif