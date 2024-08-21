/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 02:58:49 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 07:49:30 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Purpose: Implement event handling
Handles: I/O operations using kqueue, file descriptors, timers and async actions
*/

#ifndef LOOP_HPP
#define LOOP_HPP

#include <vector>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <sys/event.h>
#include <unistd.h>

#define DEFAULT 64

class FileDescriptor;
class Loop
{
	private:
	bool active;
	int kqueue_fd;
	std::vector<struct kevent> changes;
	std::vector<struct kevent> events;
	std::unordered_map<int,std::shared_ptr<FileDescriptor> > fd_map;
	std::unordered_map<int,std::pair<std::chrono::steady_clock::time_point,std::function<void()> > > timer;
	void updateKQueue(int fd, int filter, int flags, void *udata);

	public:
	Loop();
	~Loop();
	void addFD(int fd, std::shared_ptr<FileDescriptor> fd_call_map);
	void removeFD(int fd);
	void run();
	void stop();
	void actTimer(std::chrono::milliseconds timeout, std::function<void()> callback);
};

#endif