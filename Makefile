# Name

NAME = philo
NAME_BONUS = philo_bonus

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

SRCS = checks.c init.c main.c routine.c simulation.c utils.c
BONUS_SRCS = bonus/checks_bonus.c bonus/init_bonus.c bonus/main_bonus.c bonus/routine_bonus.c bonus/simulation_bonus.c bonus/utils_bonus.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Rules

all: $(NAME)

# default rule
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(MATRIX_GREEN)Executable $(NAME) created.$(RESET)"

# bonus rule
bonus: $(BONUS_OBJS)
	@$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(NAME_BONUS)
	@echo "$(MATRIX_GREEN)Executable $(NAME_BONUS) (bonus) created.$(RESET)"

# Compilation

%.o: %.c philo.h bonus/philo_bonus.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@

# Cleans

clean:
	@rm -f $(OBJS) $(BONUS_OBJS)
	@echo "$(RED)Object files of $(YELLOW)Philosophers project$(RESET) removed.$(RESET)"
fclean: clean
	@rm -f $(NAME) $(BONUS_OBJS)
	@echo "$(RED)Total clean of $(YELLOW)Philosophers project$(RESET) done.$(RESET)"
# Rebuild

re: fclean all

.PHONY: all clean fclean re bonus
