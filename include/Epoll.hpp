/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:58 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:08:59 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_H
#define EPOLL_H

#include <iostream>

#define RESET  "\033[0m"
#define YELLOW "\033[33m"

class Epoll {
	public:
		Epoll();
		Epoll(const Epoll& other);

		~Epoll();

		Epoll& operator=(const Epoll& other);

	private:
};

#endif // EPOLL_H
