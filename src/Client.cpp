/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:03 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:09:04 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {
	std::cout << YELLOW << "Default constructor called" << RESET << std::endl;
}

Client::Client(const Client& other) {
	std::cout << YELLOW << "Copy constructor called" << RESET << std::endl;
}

Client::~Client() {
	std::cout << YELLOW << "Destructor called" << RESET << std::endl;
}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		std::cout << YELLOW << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}
