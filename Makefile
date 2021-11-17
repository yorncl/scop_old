CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
CC = gcc
SRC = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJ = $(SRC:%.cpp=%.o)
NAME = scop
INC = ./inc


%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I $(INC)

all : $(NAME)


$(NAME) : $(LIBS) $(OBJ)  $(HEADERS)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -I $(INC)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re bonus run
