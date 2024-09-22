/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_functions.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:42:25 by mott              #+#    #+#             */
/*   Updated: 2024/09/21 17:28:39 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTERNAL_FUNCTIONS_H
#define EXTERNAL_FUNCTIONS_H

#include <unistd.h>			// execve, dup, dup2, pipe, fork, chdir, close, read, write, access
#include <string.h>			// strerror
#include <netdb.h>			// gai_strerror, getaddrinfo, freeaddrinfo, getprotobyname
#include <errno.h>			// errno
#include <sys/socket.h>		// socketpair, socket, accept, listen, send, recv, bind, connect, setsockopt, getsockname
#include <arpa/inet.h>		// htons, htonl, ntohs, ntohl
#include <sys/select.h>		// select
#include <poll.h>			// poll
#include <sys/epoll.h>		// epoll, epoll_create, epoll_ctl, epoll_wait
#include <sys/event.h>		// kqueue, kevent
#include <fcntl.h>			// fcntl, open
#include <sys/wait.h>		// waitpid
#include <signal.h>			// kill, signal
#include <sys/stat.h>		// stat
#include <dirent.h>			// opendir, readdir, closedir

#endif // EXTERNAL_FUNCTIONS_H
