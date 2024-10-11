

// uint32_t htonl(uint32_t hostlong);
// uint16_t htons(uint16_t hostshort);
// uint32_t ntohl(uint32_t netlong);
// uint16_t ntohs(uint16_t netshort);

// struct sockaddr {
// 	sa_family_t     sa_family;      /* Address family */
// 	char            sa_data[];      /* Socket address */
// };

// struct sockaddr_in {
// 	sa_family_t     sin_family;     /* AF_INET */
// 	in_port_t       sin_port;       /* Port number */
// 	struct in_addr  sin_addr;       /* IPv4 address */
// };

// struct in_addr {
// 	in_addr_t s_addr;
// };

// AF_INET:		For IPv4 addresses (the most commonly used option).
// AF_INET6:		For IPv6 addresses.
// AF_UNIX:		For local communication between processes on the same machine.
// SOCK_STREAM:	Provides a sequenced, reliable, two-way connection-based byte stream (e.g., TCP).
// SOCK_DGRAM:		Provides a connectionless, unreliable datagram service (e.g., UDP).
// If domain =		AF_INET and type = SOCK_STREAM, then protocol = 0 will use TCP.
// If domain =		AF_INET and type = SOCK_DGRAM, then protocol = 0 will use UDP.

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>		// socket() + socket structs
#include <netinet/in.h>		// Internet domain sockets structs
#include <arpa/inet.h>		// htons()


int main() {
	int server_fd, client_fd;
	struct sockaddr_in my_addr;
	socklen_t my_addr_size;
	const int port = 8080;
	const int backlog = 3;


	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(port);
	my_addr_size = sizeof(my_addr);

	std::cout << YELLOW << ">>>" << my_addr.sin_family << RESET << std::endl;
	std::cout << YELLOW << ">>>" << my_addr.sin_addr.s_addr << RESET << std::endl;
	std::cout << YELLOW << ">>>" << my_addr.sin_port << RESET << std::endl;

	if (bind(server_fd, (struct sockaddr*)&my_addr, my_addr_size) == -1) {
		handle_error("bind");
	}

	if (listen(server_fd, backlog) == -1) {
		handle_error("listen");
	}

	// Now we can accept incoming connections one at a time using accept().
	std::cout << "Wait for connection on port " << port << std::endl;

	client_fd = accept(server_fd, (struct sockaddr*)&my_addr, &my_addr_size);
	// client_fd = accept(server_fd, NULL, NULL);
	if (client_fd == -1) {
		handle_error("accept");
	}

	std::cout << YELLOW << ">>>" << my_addr.sin_family << RESET << std::endl;
	std::cout << YELLOW << ">>>" << my_addr.sin_addr.s_addr << RESET << std::endl;
	std::cout << YELLOW << ">>>" << my_addr.sin_port << RESET << std::endl;

	char buffer[1024] = {0};
	// recv(client_fd, buffer, sizeof(buffer), 0);
	read(client_fd, buffer, sizeof(buffer));
	std::cout << "Request received:\n" << buffer << std::endl;

	const char* http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 12\r\n"
		"\r\n"
		"Hello World!";
	// send(client_fd, http_response, strlen(http_response), 0);
	write(client_fd, http_response, strlen(http_response));

	std::cout << "HTTP response sent!" << std::endl;

	close(client_fd);
	close(server_fd);

	return 0;
}


#include <iostream>
#include <cstring>			// std::memset
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define RESET  "\033[0m"
#define YELLOW "\033[33m"
void set_nonblocking(int sockfd) {
	int flags;
	flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}


int main() {
	int server_fd;
	int client_fd;
	int epoll_fd;
	struct sockaddr_in server_addr;
	socklen_t server_addr_size;
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];
	int nfds;
	char buffer[BUFFER_SIZE];
	const char* http_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 12\r\n"
		"\r\n"
		"Hello World!";
	ssize_t count;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	set_nonblocking(server_fd);
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	server_addr_size = sizeof(server_addr);
	bind(server_fd, (struct sockaddr*)&server_addr, server_addr_size);
	listen(server_fd, SOMAXCONN);


	// epoll_fd = epoll_create(0);
	epoll_fd = epoll_create1(0);
	event.events = EPOLLIN;
	event.data.fd = server_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

	while (true) {
		std::cout << "Wait for connection" << std::endl;
		// nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 2000);

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == server_fd) {
				client_fd = accept(server_fd, NULL, NULL);
				set_nonblocking(client_fd);
				event.events = EPOLLIN | EPOLLOUT;
				event.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
			}
			else {
				count = read(events[i].data.fd, buffer, sizeof(buffer));
				if (count <= 0) {
					close(events[i].data.fd);
				}
				else {
					std::cout << "Request received:" << std::endl;
					std::cout << buffer << std::endl;
					write(client_fd, http_response, std::strlen(http_response));
				}
			}
		}
	}

	return 0;
}

// int epfd = file descriptor to epoll instance
// int op =	EPOLL_CTL_ADD
// 			EPOLL_CTL_MOD
// 			EPOLL_CTL_DEL
// struct epoll_event {
// 	uint32_t      events;  /* Epoll events */
// 	epoll_data_t  data;    /* User data variable */
// };
// union epoll_data {
// 	void     *ptr;
// 	int       fd;
// 	uint32_t  u32;
// 	uint64_t  u64;
// };
// typedef union epoll_data  epoll_data_t;
