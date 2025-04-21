# Name

NAME = philo_bonus

# Colors
RESET = \033[0m
RED = \033[31m
GREEN = \033[32m
NEON_GREEN = /033[92m
MATRIX_GREEN = \033[38;2;10;255;10m
YELLOW = \033[33m
BLUE = \033[34m

# Flags

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# Files

SRCS = checks_bonus.c init_bonus.c main_bonus.c routine_bonus.c simulation_bonus.c utils_bonus.c
OBJS = $(SRCS:.c=.o)

# Rules

all: $(NAME)

# default rule
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(MATRIX_GREEN)Executable $(NAME) created.$(RESET)"

# Compilation

%.o: %.c philo_bonus.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@

# Cleans

clean:
	@rm -f $(OBJS)
	@echo "$(RED)Object files of $(YELLOW)Philosophers project$(RESET) removed.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)Total clean of $(YELLOW)Philosophers project$(RESET) done.$(RESET)"

# Rebuild

re: fclean all

.PHONY: all clean fclean re
