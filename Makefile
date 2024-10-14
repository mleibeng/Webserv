NAME := webserv
CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++17
HEADERS := -I ./include

# Directories
SRC_DIR := src
BUILD_DIR := build

# Colors
COLOR_RESET := \033[0m
COLOR_CYAN := \033[36m
COLOR_GREEN := \033[32m
COLOR_RED := \033[31m
COLOR_YELLOW := \033[33m
COLOR_BLUE := \033[34m

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ $(HEADERS)
	@echo -e "$(COLOR_BLUE)Compiling: $(notdir $<)$(COLOR_RESET)"

$(BUILD_DIR):
	@mkdir -p $@

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $@
	@echo -e "$(YELLOW) $(CPP) $(CPPFLAGS) $(OBJS) -o $@ $(RESET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@$(CPP) $(CPPFLAGS) -MMD -MP -c $< -o $@
	@echo -e "$(BLUE) $(CPP) $(CPPFLAGS) -MMD -MP -c $< -o $@ $(RESET)"

-include $(DEPS)

$(BUILDDIR):
	@mkdir -p $@

clean:
	@echo "$(COLOR_CYAN)Cleaning compiled files$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_CYAN)Full compiled clean initiated$(COLOR_RESET)"
	@rm -f $(NAME)
	@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re