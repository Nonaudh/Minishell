/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:59:12 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:14 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_quote(char *limiter)
{
	int	i;

	i = 0;
	while (limiter[i] && limiter[i] != 34 && limiter[i] != 39)
		i++;
	if (limiter[i])
		return (1);
	return (0);
}

int	count_cmd(char **lex)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	if (!lex)
		return (0);
	if (lex[0][0] == T_NEWLINE)
		return (0);
	while (lex[i])
	{
		if (lex[i][0] == PIPE)
			count++;
		i++;
	}
	return (count);
}

int	nb_of_arg(char **lex, int x)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
	{
		if (is_a_token(lex[x][0]))
			x += 2;
		else
		{
			x++;
			count++;
		}
	}
	return (count);
}

int	set_struct_to_default(t_cmd *cmd, char **env, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		cmd[i].arg = NULL;
		cmd[i].env = env;
		cmd[i].fd_in = STDIN_FILENO;
		cmd[i].fd_out = STDOUT_FILENO;
		i++;
	}
	return (0);
}

int	until_pipe_or_newline(char **lex)
{
	int	x;

	x = 0;
	while (lex[x] && lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		x++;
	return (x);
}
