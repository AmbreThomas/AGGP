DEBUG=yes

NAME = gengraph

SRC_PATH = src
SRC_NAME = main.cpp Population.cpp Graph.cpp
SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))

OBJ_PATH = obj
OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

CC = g++
ifeq ($(DEBUG),yes)
	CFLAGS = -Wall -Werror -Wextra -g
else
	CFLAGS = -march=native -O3 -pipe
endif

INCLUDES_PATH = /usr/local/include/igraph
LIBRARY_PATH = /usr/local/lib
CPPFLAGS = -I $(INCLUDES_PATH) -I $(SRC_PATH) -L $(LIBRARY_PATH) -ligraph -lsfml-graphics -lsfml-window -lsfml-system

.PHONY:				all, clean, fclean, re

all:				$(NAME)

$(NAME):			$(OBJ)
					@echo ". : Compiling $(NAME)..."
					@export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/usr/local/lib
					@$(CC) $(OBJ) $(CPPFLAGS) -o $(NAME)
					@echo ". : Done."

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.cpp $(SRC_PATH)/%.h
					@mkdir $(OBJ_PATH) 2> /dev/null || true
					@$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

obj/main.o:			$(SRC_PATH)/main.cpp
					@mkdir $(OBJ_PATH) 2> /dev/null || true
					@$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
					@echo ". : Deleting $(NAME) objects..."
					@rm -fv $(OBJ)
					@rmdir $(OBJ_PATH) 2> /dev/null || true
					@echo ". : Done."

fclean:				clean
					@echo ". : Deleting $(NAME) executable..."
					@rm -fv $(NAME)
					@echo ". : Done."

re:					fclean all
