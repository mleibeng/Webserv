/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 08:02:15 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"

Loop::Loop() : active(false), kqueue_fd(kqueue())
{
	if (kqueue_fd == -1)
		throw std::runtime_error("Couldn't start Kqueue");

	events.reserve(DEFAULT);
	//handle even more initialization
}

Loop::~Loop()
{
	close(kqueue_fd);
}

void Loop::run()
{
	active = true;
	// while(active)
	// {
		//implement additional functionalities it should now handle while active
	// }
}

void Loop::stop()
{
	active = false;
	//Clean up resources to exit gracefully or pause running processes at the first possbility
}
void Loop::actTimer(std::chrono::milliseconds timeout, std::function<void()>)
{
	//check for a timeout
	//call function to stop a specific running process (Request/response)
}

void Loop::addFD(int fd, std::shared_ptr<FileDescriptor> fd_call_map)
{
	// add the filedescriptor to the class for processing and handling of requests
}

void Loop::removeFD(int fd)
{
	//remove fd from map of FileDescriptors,
	//can also implement automatic call when using timeout function;
}