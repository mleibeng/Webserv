/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:06 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:09:07 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"

Epoll::Epoll() {
	std::cout << YELLOW << "Default constructor called" << RESET << std::endl;
}

Epoll::Epoll(const Epoll& other) {
	std::cout << YELLOW << "Copy constructor called" << RESET << std::endl;
}

Epoll::~Epoll() {
	std::cout << YELLOW << "Destructor called" << RESET << std::endl;
}

Epoll& Epoll::operator=(const Epoll& other) {
	if (this != &other) {
		std::cout << YELLOW << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}
