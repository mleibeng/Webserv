/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:06 by mott              #+#    #+#             */
/*   Updated: 2024/10/14 20:10:46 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll() {
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1) {
		throw std::runtime_error("epoll_create1(): " + std::string(strerror(errno)));
	}
}

Epoll::~Epoll() {
	close(_epoll_fd);
}

void Epoll::add_fd(int fd, uint32_t events) {
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
		throw std::runtime_error("epoll_ctl(): " + std::string(strerror(errno)));
	}
}

int Epoll::wait(struct epoll_event* events, int max_events, int timeout) {
	return epoll_wait(_epoll_fd, events, max_events, timeout);
}

// int epoll_create(int size);
// int epoll_create1(int flags);
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *_Nullable event);
// int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
