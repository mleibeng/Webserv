/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:08:16 by mott              #+#    #+#             */
/*   Updated: 2024/10/11 15:09:09 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

int main(int argc, char** argv) {
	try {
		HTTPServer my_server;
		my_server.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
