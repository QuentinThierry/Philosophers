# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/22 00:29:38 by qthierry          #+#    #+#              #
#    Updated: 2023/08/18 20:53:49 by qthierry         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBS = -pthread
INCLUDES = -I$(HEADERS_DIR)

HEADERS_LIST = philo.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

SRC_LIST = \
			eat.c \
			ft_atoi.c \
			init.c \
			main.c \
			memory.c \
			on_death.c \
			parsing.c \
			philo_utils.c \
			print_event.c \
			philo_routine.c \
			end_checker.c \
			time.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $(LIBS) $< -o $@
	
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
