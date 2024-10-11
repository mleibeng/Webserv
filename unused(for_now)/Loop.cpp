/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/10/11 18:10:19 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"

	Loop::Loop()
	{
	#ifdef __APPLE__
		loop_fd = kqueue();
	#elif __linux__
		loop_fd = epoll_create(0);
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
	{}

	void Loop::removeFd(int fd)
	{}

	std::vector<std::pair<int, uint32_t>> Loop::wait( int timeout = -1)
	{}