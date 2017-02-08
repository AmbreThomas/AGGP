# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elopvet <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/07/18 14:13:15 by elopvet           #+#    #+#              #
#    Updated: 2016/07/27 12:26:55 by athomas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bsq

SRC_PATH = srcs
SRC_NAME = affichage.c \
		   main.c \
		   lire_fichier.c \
		   ft_argtostr.c \
		   tools.c \
		   conversion.c \
		   caractere.c \
		   resolution.c \
		   error.c \
		   resolution2.c
SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))

OBJ_PATH = obj
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

CC = gcc
CFLAGS = -Wall -Werror -Wextra

INCLUDES_PATH = includes
CPPFLAGS = -I $(INCLUDES_PATH)

.PHONY:				all, clean, fclean, re

all:				$(NAME)

$(NAME):			$(OBJ)
					$(CC) $(OBJ) -o $(NAME)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.c
					@mkdir $(OBJ_PATH) 2> /dev/null || true
					$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
					rm -fv $(OBJ)
					@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean:				clean
					rm -fv $(NAME)

re:					fclean all
