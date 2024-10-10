/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:08:16 by mott              #+#    #+#             */
/*   Updated: 2024/10/09 19:53:14 by mott             ###   ########.fr       */
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

	// system("leaks webserv");
	return 0;
}



// starten program -> server liest konfig file ein, setzt standard werte falls keine konfig file.

// machen fd auf, binden sockets, fangen an mit listen, setzten sockets auf non-blocking.

// machen event loop auf, nehmen die anfragen die kommen in die poll/kqueue/epoll

// -> schicken das an den request/response handler
// -> je nach request get/post/delete wirds anders geparst, und dann auch anders verarbeitet theoretisch kann dynamischer auch an ein CGI program das wir dann entweder asynchron oder auf nem forkedn process oder auf nem thread keine ahnung muessen wir schauen auf ausfuehren
