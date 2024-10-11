/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/12 00:44:27 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"

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
#ifdef __APPLE__
	// struct kevent event;
	// uint16_t flags = EV_ADD;
	// if (events & EPOLLET_FLAG) flags |= EV_CLEAR;
	// uint16_t filter = (events & EPOLLIN_FLAG) ? EVFILT_READ : EVFILT_WRITE;
	// EV_SET(&event, fd, filter, flags, 0, 0, nullptr);
	// change_list.push_back(event);
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
	// struct kevent event;
	// EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0 , 0, nullptr);
	// change_list.push_back(event);
	// EV_SET(&event, fd, EVFILT_WRITE, EV_DELETE, 0 , 0, nullptr);
	// change_list.push_back(event);
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
	// struct timespec ts;
	// ts.tv_sec = timeout / 1000;
	// ts.tv_nsec = (timeout % 1000) * 1000000;
	// int nev = kevent(loop_fd, change_list.data(), change_list.size(), event_list, MAX_EVENTS, timeout >= 0 ? &ts : nullptr);
	// change_list.clear();
	// for (int i = 0; i < nev; ++i)
	// {
	// 	uint32_t events = 0;
	// 	if (event_list[i].filter == EVFILT_READ) events |= EPOLLIN_FLAG;
	// 	if (event_list[i].filter == EVFILT_WRITE) events |= EPOLLOUT_FLAG;
	// 	if (event_list[i].flags & EV_EOF) events |= EPOLLHUP_FLAG;
	// 	if (event_list[i].flags & EV_ERROR) events |= EPOLLERR_FLAG;
	// 	result.emplace_back(event_list[i].ident, events);
	// }
#elif __linux__
	int nev = epoll_wait(loop_fd, event_list, MAX_EVENTS, timeout)
	for (int i = 0; i < nev; ++i)
		result.emplace_back(event_list[i].data.fd, event_list[i].events);
#endif
	return result;
}