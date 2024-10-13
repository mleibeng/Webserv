/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:58:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/13 19:14:40 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Implement event handling
Handles: I/O operations using kqueue, file descriptors, timers and async actions
*/

#ifndef LOOP_HPP
#define LOOP_HPP

#ifdef __APPLE__
#include <sys/event.h>
#define EPOLLIN_FLAG EVFILT_READ
#define EPOLLERR_FLAG EVFILT_EXCEPT
#define EPOLLHUP_FLAG EVFILT_READ
#define EPOLLOUT_FLAG EVFILT_WRITE
#define EPOLLET_FLAG EV_CLEAR
#else
#include <sys/epoll.h>
#define EPOLLIN_FLAG EPOLLIN
#define EPOLLERR_FLAG EPOLLERR
#define EPOLLHUP_FLAG EPOLLHUP
#define EPOLLOUT_FLAG EPOLLOUT
#define EPOLLET_FLAG EPOLLET
#endif

#include <vector>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <unistd.h>

class FileDescriptor;
class Loop
{
	private:
	int loop_fd;
	static const int MAX_EVENTS = 10;
#ifdef __APPLE__
	// std::vector<struct kevent> change_list;
	// struct kevent event_list[MAX_EVENTS];
#else
	std::unordered_map<int, epoll_event> events;
	epoll_event event_list[MAX_EVENTS];
#endif

	public:
	Loop();
	~Loop();
	void addFd(int fd, uint32_t events);
	void removeFd(int fd);
	std::vector<std::pair<int, uint32_t>> wait( int timeout = -1);

};

#endif