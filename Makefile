

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
CC = gcc
SRC = $(wildcard src/*.c)
LDFLAGS= -ldl -lm -L ./lib/libglfw3 -lglfw3 -L ./lib/libft -lft
HEADERS = $(wildcard src/*.h)
OBJ = $(SRC:%.c=%.o)
NAME = scop
INC = ./include
LIBFT = lib/libft/libft.a

ifeq ($(OS),Windows_NT)
	@echo Build not supported on Windows
	exit 1
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS += -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi
    endif
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS += -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
    endif
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I $(INC) $(LDFLAGS)
%/glad.o: %/glad.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I $(INC)

all : $(NAME)

$(LIBFT): 
	$(MAKE) -C ./lib/libft

clean_libft: 
	$(MAKE) -C ./lib/libft fclean

$(NAME) : $(LIBFT) $(OBJ)  $(HEADERS)
	$(CC) $(CFLAGS)  $(OBJ) $(LIBFT) -o $(NAME) -I $(INC) $(LDFLAGS)

clean: clean_libft
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re bonus run libft
