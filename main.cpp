/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:08:16 by mott              #+#    #+#             */
/*   Updated: 2024/10/09 16:41:56 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

int main() {
	try {
		HTTPServer my_server;
		my_server.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
