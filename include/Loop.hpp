/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:58:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/12/03 16:36:51 by mott             ###   ########.fr       */
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
		std::vector<std::pair<int, uint32_t>> wait(int timeout = -1);
		void modifyFd(int fd, uint32_t event);
		bool hasFd(int fd) const;

	private:
		int loop_fd;
		static const int MAX_EVENTS = 1024;
		std::unordered_map<int, epoll_event> events;
		epoll_event event_list[MAX_EVENTS];
};

#endif
