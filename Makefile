# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/15 17:49:39 by jjs               #+#    #+#              #
#    Updated: 2025/01/02 15:29:38 by jslusark         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED = \033[0;31m
GRAY = \033[0;90m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
RESET = \033[0;97m

# Messages
SUCCESS = $(GREEN)Success:$(RESET)
WARNING = $(YELLOW)Warning:$(RESET)
FAILURE = $(RED)Failure:$(RESET)
IDLE = $(BLUE)Idle:$(RESET)

# Compiler flags and commands
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
REMOVE = rm -rf

# Source files
SRC = main.c \
	utils.c
OBJ = $(SRC:.c=.o)

# Compile object files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

#Executable name
NAME = program

# Default target
all: $(NAME)
	@echo "$(IDLE) $(MAGENTA)$(NAME)$(RESET) is up to date!"

# Compile the main executable
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -I. $(OBJ) -o $(NAME)
	@echo "$(SUCCESS) $(MAGENTA)$(NAME)$(RESET) archived and indexed!"

# Clean object files
clean:
	@if ls $(OBJ) >/dev/null 2>&1; then \
		$(REMOVE) $(OBJ); \
		echo "$(WARNING) removed $(MAGENTA)$(NAME)$(RESET) object files"; \
	else \
		echo "$(IDLE) object files were already cleaned"; \
	fi

# Full clean
fclean: clean
	@if [ -f $(NAME) ]; then \
		$(REMOVE) $(NAME); \
		echo "$(WARNING) removed $(MAGENTA)$(NAME)$(RESET) executable"; \
	else \
		echo "$(IDLE) Executables and object files were already cleaned"; \
	fi

# Recompile everything
re: fclean all
	@echo "$(WARNING) recompiled everything from $(MAGENTA)$(NAME)$(RESET)"

%:
	@echo "$(FAILURE) target '$@' is not a valid target.$(RESET)"
	@echo "Please use one of the following valid targets:"
	@echo "- $(GREEN)make$(RESET): compiles the project"
	@echo "- $(GREEN)make re$(RESET): recompiles the project"
	@echo "- $(GREEN)make clean$(RESET): removes object files"
	@echo "- $(GREEN)make fclean$(RESET): removes object files and the final library"

.PHONY: all clean fclean re %
