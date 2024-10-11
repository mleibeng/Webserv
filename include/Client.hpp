/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:08:55 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:08:56 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#define RESET  "\033[0m"
#define YELLOW "\033[33m"

class Client {
	public:
		Client();
		Client(const Client& other);

		~Client();

		Client& operator=(const Client& other);

	private:
};

#endif // CLIENT_H
