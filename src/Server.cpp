/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:13 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:09:14 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
	std::cout << YELLOW << "Default constructor called" << RESET << std::endl;
}

Server::Server(const Server& other) {
	std::cout << YELLOW << "Copy constructor called" << RESET << std::endl;
}

Server::~Server() {
	std::cout << YELLOW << "Destructor called" << RESET << std::endl;
}

Server& Server::operator=(const Server& other) {
	if (this != &other) {
		std::cout << YELLOW << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}
