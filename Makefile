CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
CC = gcc
SRC = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJ = $(SRC:%.c=%.o)
NAME = scop
INC = ./inc
LIBFT = lib/libft/libft.a


%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I $(INC)

all : $(NAME)

$(LIBFT): 
	$(MAKE) -C ./lib/libft

clean_libft: 
	$(MAKE) -C ./lib/libft fclean

$(NAME) : $(LIBFT) $(OBJ)  $(HEADERS)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -I $(INC)

clean: clean_libft
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re bonus run libft
