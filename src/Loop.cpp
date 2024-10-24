/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/18 16:21:30 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <iostream>

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

Loop::~Loop()
{
	if (loop_fd != -1)
		close(loop_fd);
}

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
	std::cout << fd << std::endl;
// #endif
}

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
