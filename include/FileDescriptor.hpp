/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:57:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 07:29:40 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Base Class for handling of File Descriptors
Handles: Interface for read/write operations and error handling, allows setting of FDs to non-blocking
*/

#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP

#include <memory>
#include <functional>

class Loop;
class FileDescriptor
{
	protected:
	Loop *loop;
	int fd;

	public:
	FileDescriptor(Loop *loop, int fd);
	virtual ~FileDescriptor();

	virtual void handleRead() = 0;
	virtual void handleWrite() = 0;
	virtual void handleError() = 0;
	int getFd();
	void setNonBlock();
};

#endif