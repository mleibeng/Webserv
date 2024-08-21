/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:22 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 07:51:06 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

void Connection::handleRead()
{
	//Use this to read in request using parse function.
}

void Connection::handleWrite()
{

}

void Connection::handleError()
{

}

// should I make the processing of requests part of the class
// or implement the functionality in connection?
void Connection::processRequest()
{
	// if (request == Method::GET)
	// 	request.getRequest();
	// else if (request == Method::POST)
	//	request.postRequest();
	// else if (request == Method::DELETE)
	// request.delRequest();
}

Connection::Connection(Loop *loop, int fd) : FileDescriptor(loop, fd), request(), response(),
											 buf(), out_buf()
{
	setNonBlock();
	loop->addFD(fd, shared_from_this());
}