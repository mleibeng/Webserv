/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/12 03:16:06 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <iostream>

Loop::Loop()
{
#ifdef __APPLE__
	loop_fd = kqueue();
#elif __linux__
	loop_fd = epoll_create1(0);
#endif
	if (loop_fd == -1)
		throw std::runtime_error("Couldn't Create loop fd");
}

Loop::~Loop()
{
	if (loop_fd != -1)
		close(loop_fd);
}

void Loop::addFd(int fd, uint32_t events)
{
	(void)fd;
	(void)events;
	std::cout << "trying to add fd to epoll_event/kevent" << std::endl;
#ifdef __APPLE__
	//dont know yet
#elif __linux__
	epoll_event ev
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(loop_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("Couldn't add fd to epoll");
	events[fd] = ev;
#endif
}

void Loop::removeFd(int fd)
{
	(void)fd;
#ifdef __APPLE__
	// don't know yet
#elif __linux__
	epoll_ctl(loop_fd, EPOLL_CTL_DEL, fd, nullptr);
	events.erase(fd);
#endif
}

std::vector<std::pair<int, uint32_t>> Loop::wait(int timeout)
{
	(void)timeout;
	std::vector<std::pair<int, uint32_t>> result;
#ifdef __APPLE__
	// don't know yet
#elif __linux__
	int nev = epoll_wait(loop_fd, event_list, MAX_EVENTS, timeout)
	for (int i = 0; i < nev; ++i)
		result.emplace_back(event_list[i].data.fd, event_list[i].events);
#endif
	return result;
}