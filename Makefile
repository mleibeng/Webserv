NAME		:= webserv
CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror
HEADERS 	:= -I ./include

# Directories
SRC_DIR := srcs
OBJ_DIR := obj

COLOR_RESET			=	\033[0m
COLOR_CYAN			=	\033[36m
COLOR_GREEN			=	\033[32m
COLOR_RED			=	\033[31m
COLOR_YELLOW		=	\033[33m

SRCS	:= $(wildcard $(SRC_DIR)/*.cpp)
OBJS	:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "$(COLOR_YELLOW)Compiling: $(notdir $<)$(COLOR_RESET)\n"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(HEADERS) -o $(NAME)
	@echo "$(COLOR_GREEN)Compilation for $(NAME) complete$(COLOR_RESET)"

clean:
	@echo "$(COLOR_CYAN)Cleaning compiled files$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_CYAN)Full compiled clean initiated$(COLOR_RESET)"
	@rm -rf $(NAME)
	@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re
