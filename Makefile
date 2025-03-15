# Name

NAME = philo

# Flags

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# Files

SRCS = main.c routine.c utils.c
OBJS = $(SRCS:.c=.o)

# Rules

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Compilation

%.o: %.c philo.h Makefile
	$(CC) $(CFLAGS) -c $< -o $@

# Cleans

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

# Rebuild

re: fclean all

.PHONY: all clean fclean re
