NAME = gengraph

SRC_PATH = src
SRC_NAME = Population.cpp \
		   main.cpp \
		   Graph.cpp
SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))

OBJ_PATH = obj
OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

CC = g++
CFLAGS = -Wall -Werror -Wextra

INCLUDES_PATH = /usr/local/include/igraph
LIBRARY_PATH = /usr/local/lib
CPPFLAGS = -I $(INCLUDES_PATH) -I $(SRC_PATH) -L $(LIBRARY_PATH) -ligraph

.PHONY:				all, clean, fclean, re

all:				$(NAME)

$(NAME):			$(OBJ)
					export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
					$(CC) $(OBJ) $(CPPFLAGS) -o $(NAME)

$(OBJ_PATH)/%.o:	$(SRC_PATH)/%.cpp $(SRC_PATH)/%.h
					@mkdir $(OBJ_PATH) 2> /dev/null || true
					$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
					
obj/main.o:			$(SRC_PATH)/main.cpp
					@mkdir $(OBJ_PATH) 2> /dev/null || true
					$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
					rm -fv $(OBJ)
					@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean:				clean
					rm -fv $(NAME)

re:					fclean all
