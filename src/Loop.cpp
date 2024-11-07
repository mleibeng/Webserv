/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 03:00:30 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/07 03:36:51 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <iostream>

bool Loop::hasFd(int fd) const
{
	return events.find(fd) != events.end();
}

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
	try
	{
		epoll_event ev = {};
		// Add EPOLLET for edge-triggered mode
		// Add EPOLLRDHUP to detect peer shutdown
		ev.events = event | EPOLLET | EPOLLRDHUP;
		ev.data.fd = fd;

		if (epoll_ctl(loop_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
			throw std::runtime_error("Couldn't add fd to epoll: " + std::string(strerror(errno)));
		}

		events[fd] = ev;
		std::cout << "Added fd: " << fd << " to epoll" << std::endl;
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Failed to add fd: " + std::to_string(fd) + " - " + e.what());
	}
// #endif
}

 void Loop::modifyFd(int fd, uint32_t event)
 {
	epoll_event ev = {};
	ev.events = event | EPOLLET | EPOLLRDHUP;
	ev.data.fd = fd;

	if (epoll_ctl(loop_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
		throw std::runtime_error("Couldn't modify fd in epoll");
	}
	events[fd] = ev;
}

/// @brief removes a file descriptor from event list
/// @param fd file descriptor to remove
void Loop::removeFd(int fd)
{
	if (epoll_ctl(loop_fd, EPOLL_CTL_DEL, fd, nullptr) == -1)
	{
		// Only throw if the error isn't that the fd doesn't exist
		if (errno != ENOENT)
		{
			throw std::runtime_error("Couldn't remove fd from epoll: " + std::string(strerror(errno)));
		}
	}
	events.erase(fd);
	std::cout << "Removed fd: " << fd << " from epoll" << std::endl;
}

/// @brief wait for event to happen on registered file descriptors
/// @param timeout time to wait before closing file descriptor in ms (not yet utilized)
/// @return returns a vector of a pair of file descriptor/events being waited on.
std::vector<std::pair<int, uint32_t>> Loop::wait(int timeout)
{
	std::vector<std::pair<int, uint32_t>> result;

	int nev = epoll_wait(loop_fd, event_list, MAX_EVENTS, timeout);
	if (nev == -1) {
		if (errno == EINTR) {
			// Interrupted by signal, just return empty result
			return result;
		}
		throw std::runtime_error("epoll_wait failed: " + std::string(strerror(errno)));
	}

	for (int i = 0; i < nev; ++i) {
		int fd = event_list[i].data.fd;
		uint32_t events = event_list[i].events;

		// Handle errors and hangups
		if (events & (EPOLLERR | EPOLLHUP)) {
			// Add to result so the main loop can handle cleanup
			result.emplace_back(fd, events);
			continue;
		}

		// Handle normal events
		result.emplace_back(fd, events);
	}

	return result;
}
