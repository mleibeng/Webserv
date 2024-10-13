/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 02:43:00 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/09 21:32:48 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Manage client connections
Handles: FileDescriptor Inheritance for socket I/O handling & Processing of incoming requests and responses.
*/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "FileDescriptor.hpp"
#include "Loop.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include <array>

#define ARRAY_SIZE 8192

class Connection : public FileDescriptor, public std::enable_shared_from_this<Connection>
{
	private:
	// HttpRequest request;
	// HttpResponse response;
	std::array<char, ARRAY_SIZE> buf;
	std::string out_buf; //need to ensure this can handle potentially large data volumes

	public:
	Connection(Loop *loop, int fd);

	void handleRead() override;
	void handleWrite() override;
	void handleError() override;

	void processRequest();
};

#endif