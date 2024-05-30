#ifndef MINISHELL_H
# define MINISHELL_H

#include "../lib/libft/libft.h"

#include <stdio.h>
#include <fcntl.h>

# define BIG_BUFFER 100

typedef enum e_token
{
	LEX = 1,
	GREAT,
	LESS,
	PIPE,
} t_token;

void	print_tab(char **tab);
void	free_the_tab(char **tab);
char	**ft_str_tab_dup(char **tab1);

# endif