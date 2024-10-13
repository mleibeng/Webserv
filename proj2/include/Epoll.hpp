/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:58 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 17:46:39 by mott             ###   ########.fr       */
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
