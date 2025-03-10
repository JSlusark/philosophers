# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/15 17:49:39 by jjs               #+#    #+#              #
#    Updated: 2025/03/10 12:43:42 by jslusark         ###   ########.fr        #
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
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
CFLAGS = -Wall -Wextra -Werror -g
REMOVE = rm -rf

# Source files
SRC = main.c \
	init.c \
	routine.c \
	utils.c
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Compile object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

#Executable name
NAME = philo

# Default target
all: $(NAME)
	@echo "$(IDLE) $(MAGENTA)$(NAME)$(RESET) is up to date!"

# Compile the main executable
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -I. $(OBJ) -o $(NAME)
	@echo "$(SUCCESS) $(MAGENTA)$(NAME)$(RESET) archived and indexed!"

# Clean object files
clean:
	@if ls $(OBJ_DIR)/*.o >/dev/null 2>&1; then \
		$(REMOVE) $(OBJ_DIR); \
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

# Calling to check for data races
# make helgrind/drd ARGS="4 310 200 200"
helgrind:
	@valgrind --tool=helgrind ./$(NAME) $(ARGS)

drd:
	@valgrind --tool=drd --check-stack-var=yes ./$(NAME) $(ARGS)
