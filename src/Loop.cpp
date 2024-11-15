/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/15 03:50:04 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <iostream>

/// @brief create epoll/kqueue file descriptor / throw error in case of failure
Loop::Loop()
{
// #ifdef __APPLE__
	// loop_fd = kqueue();
// #else
	loop_fd = epoll_create1(0);
// #endif
	if (loop_fd == -1)
		throw std::runtime_error("Couldn't Create loop fd");
}

/// @brief close epoll/kqueue file descriptor
Loop::~Loop()
{
	if (loop_fd != -1)
		close(loop_fd);
}

/// @brief add file descriptor to epoll/kqueue event
/// @param fd file descriptor to add to eventlist
/// @param event event list for processing
void Loop::addFd(int fd, uint32_t event)
{
// #ifdef __APPLE__
	//dont know yet
// #else
	epoll_event ev;
	ev.events = event;
	ev.data.fd = fd;
	if (epoll_ctl(loop_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("Couldn't add fd to epoll");
	events[fd] = ev;
	// std::cout << fd << std::endl;
// #endif
}

/// @brief removes a file descriptor from event list
/// @param fd file descriptor to remove
void Loop::removeFd(int fd)
{
	// (void)fd;
// #ifdef __APPLE__
	// don't know yet
// #else
	epoll_ctl(loop_fd, EPOLL_CTL_DEL, fd, nullptr);
	events.erase(fd);
// #endif
}

/// @brief wait for event to happen on registered file descriptors
/// @param timeout time to wait before closing file descriptor in ms (not yet utilized)
/// @return returns a vector of a pair of file descriptor/events being waited on.
std::vector<std::pair<int, uint32_t>> Loop::wait(int timeout)
{
	// (void)timeout;
	std::vector<std::pair<int, uint32_t>> result;
// #ifdef __APPLE__
	// don't know yet
// #else
	int nev = epoll_wait(loop_fd, event_list, MAX_EVENTS, timeout);
	for (int i = 0; i < nev; ++i)
	{
		int fd = event_list[i].data.fd;
		uint32_t events = event_list[i].events;
		result.emplace_back(fd, events);
	}
// #endif
	return result;
}
