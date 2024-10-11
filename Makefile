# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/20 13:55:42 by mott              #+#    #+#              #
#    Updated: 2024/10/11 14:29:22 by mott             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror
CPPFLAGS	+=	-Iinclude
# CPPFLAGS	+=	-g -fsanitize=address
# CPPFLAGS	+=	-std=c++98
CPPFLAGS	+=	-std=c++17
RM			=	rm -rf
NAME		=	webserv

SRCDIR		=	src
BUILDDIR	=	build

SRCS		=	$(SRCDIR)/main.cpp \
				$(SRCDIR)/Client.cpp \
				$(SRCDIR)/Epoll.cpp \
				$(SRCDIR)/Server.cpp \
OBJS		=	$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS		=	$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.d,$(SRCS))

RESET	=	\x1b[0m
RED		=	\x1b[31m
YELLOW	=	\x1b[33m
BLUE	=	\x1b[34m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $@
	@echo "$(YELLOW) $(CPP) $(CPPFLAGS) $(OBJS) -o $@ $(RESET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@$(CPP) $(CPPFLAGS) -MMD -MP -c $< -o $@
	@echo "$(BLUE) $(CPP) $(CPPFLAGS) -MMD -MP -c $< -o $@ $(RESET)"

-include $(DEPS)

$(BUILDDIR):
	@mkdir -p $@

clean:
	@$(RM) $(BUILDDIR)
	@echo "$(RED) $(RM) $(BUILDDIR) $(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) $(RM) $(NAME) $(RESET)"

re: fclean all

.PHONY: all clean fclean re
