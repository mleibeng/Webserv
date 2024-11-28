/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:58:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/28 04:02:08 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Implement event handling
Handles: I/O operations using kqueue, file descriptors, timers and async actions
*/

#ifndef LOOP_HPP
#define LOOP_HPP

#include "HeaderIncludes.hpp"

class Loop
{
	public:
		Loop();
		~Loop();
		void addFd(int fd, uint32_t events);
		void removeFd(int fd);
		std::vector<std::pair<int, uint32_t>> wait( int timeout = -1);
		void modifyFd(int fd, uint32_t event);
		bool hasFd(int fd) const;

	private:
		int loop_fd;
		static const int MAX_EVENTS = 1024;
		#ifdef __APPLE__
			// std::vector<struct kevent> change_list;
			// struct kevent event_list[MAX_EVENTS];
		#else
			std::unordered_map<int, epoll_event> events;
			epoll_event event_list[MAX_EVENTS];
		#endif
};

#endif
