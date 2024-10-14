/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:58 by mott              #+#    #+#             */
/*   Updated: 2024/10/14 20:10:05 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_H
#define EPOLL_H

#include "external_functions.hpp"

class Epoll {
	public:
		Epoll();
		~Epoll();

		Epoll(const Epoll& other) = delete;
		Epoll& operator=(const Epoll& other) = delete;

		void add_fd(int fd, uint32_t events);
		int wait(struct epoll_event* events, int max_events, int timeout = -1);

	private:
		int _epoll_fd;
};

#endif // EPOLL_H
