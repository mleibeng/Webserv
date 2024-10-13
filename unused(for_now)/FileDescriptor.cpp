/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 08:01:48 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileDescriptor.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

FileDescriptor::FileDescriptor(Loop *loop, int fd) : loop(loop), fd(fd)
{}

FileDescriptor::~FileDescriptor()
{
	close(fd);
}

int FileDescriptor::getFd()
{
	return fd;
}

// ONLY F_SETFL, O_NONBLOCK and FD_CLOEXEC ALLOWED.
void FileDescriptor::setNonBlock()
{
	int flags = fcntl(fd, F_SETFL, O_NONBLOCK, O_CLOEXEC);
	if (flags == -1)
		throw std::runtime_error("Could not set FD non-blocking");
}