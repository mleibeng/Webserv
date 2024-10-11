/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:08:16 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 17:42:21 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Epoll.hpp"

#define PORT 8080
// Firefox -> http://localhost:8080
// Firefox -> http://127.0.0.1:8080

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	try {
		Server my_server(PORT);
		my_server.start();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
