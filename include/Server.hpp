/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:09:00 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:09:01 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#define RESET  "\033[0m"
#define YELLOW "\033[33m"

class Server {
	public:
		Server();
		Server(const Server& other);

		~Server();

		Server& operator=(const Server& other);

	private:
};

#endif // SERVER_H
