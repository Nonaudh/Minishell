CC = cc

CFLAGS = -g -Wall -Wextra -Werror 

SRC = src/main.c 			src/builtins/builtins_utils.c	src/builtins/ft_exit_utils.c \
src/builtins/ft_export_2.c	src/builtins/ft_export.c		src/lexing/utils_2.c \
src/builtins/ft_export_3.c	src/builtins/ft_pwd.c			src/lexing/utils_3.c \
src/builtins/ft_export_4.c	src/builtins/ft_unset.c			src/lexing/utils.c \
src/builtins/ft_cd_2.c		src/execution/exec_builtins.c	src/parsing/argument.c \
src/builtins/ft_cd.c		src/execution/exec_child.c		src/parsing/error.c \
src/builtins/ft_cd_utils.c	src/execution/execution.c		src/parsing/here_doc.c \
src/builtins/ft_echo.c		src/execution/utils.c			src/parsing/parsing.c \
src/builtins/ft_env.c		src/lexing/expansion.c			src/parsing/signal.c \
src/builtins/ft_exit.c		src/lexing/lexing.c				src/parsing/utils.c	\
src/lexing/token.c			src/signal.c
		
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
