# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmarinov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/27 13:09:50 by mmarinov          #+#    #+#              #
#    Updated: 2025/02/18 20:33:59 by mmarinov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors

END     = \033[0m
YELLOW  = \033[0;93m
GREEN   = \033[0;32m
RED     = \033[0;91m
CYAN    = \033[0;96m
BLUE    = \033[0;94m

# Project settings

NAME    = philo
SRC_PATH = src/
INC_PATH = inc/

# Source files and object files

SRC     = main.c utils.c habits.c inits.c monitor.c parseo.c
SRCS    = $(addprefix $(SRC_PATH), $(SRC))
OBJ_DIR = objs
OBJS    = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Compiler and flags

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -I$(INC_PATH)
CFLAGS += -g -O1 #-fsanitize=thread
LDFLAGS = -lpthread
RM      = rm -rf

# Targets

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(YELLOW)Executable $(NAME) created successfully!$(END)"

$(OBJ_DIR)/%.o: $(SRC_PATH)%.c $(INC_PATH)/philo.h Makefile
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled $<$(END)" > /dev/null 2>&1

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(BLUE)Cleaned up object & dependency files!$(END)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Executable $(NAME) removed!$(END)"

re: fclean all
	@echo "$(CYAN)Everything clean & recompiled!$(END)"

.PHONY: all clean fclean re
