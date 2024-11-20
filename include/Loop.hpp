/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:58:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/07 03:36:48 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Implement event handling
Handles: I/O operations using kqueue, file descriptors, timers and async actions
*/

#ifndef LOOP_HPP
#define LOOP_HPP

#include "HeaderIncludes.hpp"

class Client;
class HttpRequest;
class HttpResponse;
class RequestHandler;
class Config;

enum class ClientState 
{
	READING_HEADERS,
	READING_BODY,
	PROCESSING,
	SENDING_RESPONSE,
	COMPLETE
};


struct ClientConnection 
{
	std::unique_ptr<Client> client;
	std::vector<char> read_buffer;
	std::string pending_response;
	size_t byte_read;
	size_t byte_written;
	ClientState state;
	bool keep_alive;

	ClientConnection(int fd, const Config& config, size_t buffer_size);
};

class Loop
{
	public:
		Loop(const Config& conf);
		~Loop();
		void addFd(int fd, uint32_t events);
		void removeFd(int fd);
		std::vector<std::pair<int, uint32_t>> wait( int timeout = -1);
		void modifyFd(int fd, uint32_t event);
		bool hasFd(int fd) const;
		void addClient(int fd, size_t buffersize);
		void processEvents(RequestHandler& handler, int timeout);

	private:
		int loop_fd;
		std::unordered_map<int, std::unique_ptr<ClientConnection>> connections;
		static const int MAX_EVENTS = 10;
		const Config& config;
		#ifdef __APPLE__
			// std::vector<struct kevent> change_list;
			// struct kevent event_list[MAX_EVENTS];
		#else
			std::unordered_map<int, epoll_event> events;
			epoll_event event_list[MAX_EVENTS];
		#endif

		bool isHeaderComplete(const std::vector<char>& buffer, size_t byte_read);
		void handleRead(int fd, ClientConnection& conn);
		void handleWrite(int fd, ClientConnection& conn);
		void processRequest(int fd, ClientConnection& conn, RequestHandler& handler);		
};

#endif
