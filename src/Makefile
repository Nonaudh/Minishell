CC = cc

CFLAGS = -g #-Wall -Wextra -Werror 

SRC = signal.c
		
OBJ = $(SRC:%.c=%.o)

INCLUDE = -L../lib/libft -lft -L../inc/minishell -lreadline

LIBFTDIR = ../lib/libft

NAME = signal

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