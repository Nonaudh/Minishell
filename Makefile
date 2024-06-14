CC = cc

CFLAGS = -g #-Wall -Wextra -Werror 

SRC = src/main.c src/lexing/lexing.c src/lexing/token.c src/lexing/expansion.c \
	src/lexing/utils.c src/lexing/utils_2.c src/parsing/parsing.c
		
OBJ = $(SRC:%.c=%.o)

INCLUDE = -Llib/libft -lft -Linc/minishell -lreadline

LIBFTDIR = lib/libft

NAME = minishell

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} 

$(NAME) : $(OBJ) $(OBJ_BONUS) 
	$(MAKE) -sC $(LIBFTDIR)
	@$(CC) $(OBJ) $(INCLUDE) -o $(NAME)
	@echo $(NAME) compiled

all : $(NAME)

clean :
	rm -f $(OBJ) $(OBJ_BONUS)
	$(MAKE) -sC $(LIBFTDIR) clean

fclean : clean
	rm -f $(NAME)
	$(MAKE) -sC $(LIBFTDIR) fclean

re: fclean all

.PHONY : all clean fclean re