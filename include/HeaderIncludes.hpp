/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderIncludes.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 01:10:59 by mleibeng          #+#    #+#             */
/*   Updated: 2024/11/15 02:59:13 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_INCLUDES
#define HEADER_INCLUDES

#ifdef __APPLE__
#include <sys/event.h>
#define EPOLLIN_FLAG EVFILT_READ
#define EPOLLERR_FLAG EVFILT_EXCEPT
#define EPOLLHUP_FLAG EVFILT_READ
#define EPOLLOUT_FLAG EVFILT_WRITE
#define EPOLLET_FLAG EV_CLEAR
#else
#include <sys/epoll.h>
#define EPOLLIN_FLAG EPOLLIN
#define EPOLLERR_FLAG EPOLLERR
#define EPOLLHUP_FLAG EPOLLHUP
#define EPOLLOUT_FLAG EPOLLOUT
#define EPOLLET_FLAG EPOLLET
#endif

#include <vector>
#include <unordered_map>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <fstream>
#include <optional>
#include <variant>
#include <filesystem>
#include <exception>
#include <sys/wait.h>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <thread>
#include <algorithm>

#endif