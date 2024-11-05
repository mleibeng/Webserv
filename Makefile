# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 13:55:42 by mott              #+#    #+#              #
#    Updated: 2024/11/05 01:37:40 by mleibeng         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Guide to using this Makefile:
#	Call make dev-start to build the container, start it up and enter it automatically using bash
#	Call make to create the project
#	use ./(NAME) to execute the program

#	call make clean/fclean to delete project files or make re to rebuild project files
#	call make docker-re to stop the container, rebuild and reenter it

# Compiler and flags
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -Iinclude -std=c++17
# CPPFLAGS += -g -fsanitize=address
RM = rm -rf

# Project names and directories
NAME = webserv
SRCDIR = src
BUILDDIR = build

# Source and object files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.d,$(SRCS))

# Colors
RESET = \x1b[0m
RED = \x1b[31m
YELLOW = \x1b[33m
BLUE = \x1b[34m
GREEN = \x1b[32m

# Docker settings
WORKSPACE := $(CURDIR)

# C++ build rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $@
	@echo -e "$(YELLOW)Built: $(NAME)$(RESET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@$(CPP) $(CPPFLAGS) -MMD -MP -c $< -o $@
	@echo -e "$(BLUE)Compiled: $<$(RESET)"

-include $(DEPS)

$(BUILDDIR):
	@mkdir -p $@

# Clean rules
clean:
	@$(RM) $(BUILDDIR)
	@echo -e "$(RED)Cleaned build directory$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo -e "$(RED)Removed executable$(RESET)"

re: fclean all

# Docker rules
docker-build:
	@echo -e "$(BLUE)Building Docker image...$(RESET)"
	@docker build -t webserver .
	@echo -e "$(GREEN)Docker image built successfully$(RESET)"

docker-up:
	@echo -e "$(BLUE)Starting Docker container...$(RESET)"
	@docker run -itd \
		--name WebServer \
		-v $(CURDIR):/workspace \
		-p 8080:8080 \
		-p 8081:8081 \
		webserver
	@echo -e "$(GREEN)Docker container started$(RESET)"

docker-down:
	@echo -e "$(BLUE)Stopping Docker container...$(RESET)"
	@docker stop WebServer || true
	@echo -e "$(GREEN)Docker container stopped$(RESET)"

docker-clean: docker-down
	@echo -e "$(BLUE)Removing Docker container and image...$(RESET)"
	@docker rm WebServer || true
	@docker rmi webserver || true
	@echo -e "$(GREEN)Docker cleanup complete$(RESET)"

docker-shell:
	@echo "Entering Docker Container"
	@docker exec -it WebServer /bin/bash

dev-start: docker-build docker-up docker-shell

docker-re: docker-clean dev-start

# Combined rules

.PHONY: all clean fclean re \
	docker-build docker-up docker-down docker-clean \
	docker-start docker-re dev-start