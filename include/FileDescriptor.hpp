/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:57:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/20 07:05:25 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Base Class for handling of File Descriptors
Handles: Interface for read/write operations and error handling, allows setting of FDs to non-blocking
*/

#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP

class FileDescriptor
{
	public:
	FileDescriptor();
	~FileDescriptor();

	virtual void handleRead() = 0;
	virtual void handleWrite() = 0;
	virtual void handleError() = 0;
	int getFd();
	void setNonBlock();
};

#endif