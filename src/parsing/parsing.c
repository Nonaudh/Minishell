/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:58:56 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:58:58 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	piped(t_cmd *cmd_in, t_cmd *cmd_out)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd))
		printf("Error pipe\n");
	if (cmd_in->fd_out == STDOUT_FILENO)
		cmd_in->fd_out = pipe_fd[1];
	else
		close(pipe_fd[1]);
	if (cmd_out->fd_in == STDIN_FILENO)
		cmd_out->fd_in = pipe_fd[0];
	else
		close(pipe_fd[0]);
	return (1);
}

int	put_in_the_struct(char **lex, t_cmd *cmd, int *ex_st)
{
	if (lex[0][0] == LESS || lex[0][0] == LESSLESS)
		return (less_infile(lex, cmd, ex_st));
	if (lex[0][0] == GREAT)
		return (great_outfile(lex, cmd, ex_st));
	if (lex[0][0] == GREATGREAT)
		return (great_great_outfile(lex, cmd, ex_st));
	return (is_an_argument(lex, cmd));
}

t_cmd	*fill_the_struct(char **lex, t_cmd *cmd, int size, int *ex_st)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	x = 0;
	y = 0;
	while (y != -1 && lex[x][0] != T_NEWLINE)
	{
		while (y != -1 && lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		{
			y = put_in_the_struct(&lex[x], &cmd[i], ex_st);
			x += y;
		}
		if (y != -1 && lex[x][0] == PIPE)
		{
			y = piped(&cmd[i], &cmd[i + 1]);
			x += y;
		}
		i++;
	}
	if (y == -1)
		return (error_struct_cmd(cmd, size, ex_st));
	return (cmd);
}

t_cmd	*create_struct_cmd(char **lex, char **env, int size, int *ex_st)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd) * (size + 1));
	if (!cmd)
		return (NULL);
	set_struct_to_default(cmd, env, size);
	if (!open_here_doc(lex, cmd, env, ex_st))
		cmd = fill_the_struct(lex, cmd, size, ex_st);
	else
	{
		free_struct_cmd(cmd, size);
		cmd = NULL;
	}
	return (cmd);
}

t_cmd	*parsing(char **lex, char **env, int size, int *ex_st)
{
	t_cmd	*cmd;

	cmd = create_struct_cmd(lex, env, size, ex_st);
	return (cmd);
}
