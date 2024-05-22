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

# endif