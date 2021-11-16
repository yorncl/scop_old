CFLAGS = -Wall -Wextra -Werror
CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
NAME = scop
INC = ./inc


%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I $(INC)

all : $(NAME)


$(NAME) : $(LIBS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -I $(INC)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: all

run: $(NAME)
	./$(NAME)

.PHONY: all $(NAME) clean fclean re bonus run
