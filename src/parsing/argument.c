/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:58:29 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:58:38 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	less_infile(char **lex, t_cmd *cmd, int *ex_st)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (lex[1][0] == T_EMPTY)
	{
		error_ambiguous_redirect(ex_st);
		cmd->fd_out = -1;
		return (until_pipe_or_newline(lex));
	}
	cmd->fd_in = open(lex[1], O_RDONLY);
	if (cmd->fd_in == -1)
	{
		error_open_file(lex[1], ex_st);
		return (until_pipe_or_newline(lex));
	}
	if (lex[0][0] == LESSLESS)
	{
		if (unlink(lex[1]))
			perror("unlink : here_doc");
	}
	return (2);
}

int	great_outfile(char **lex, t_cmd *cmd, int *ex_st)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (lex[1][0] == T_EMPTY)
	{
		error_ambiguous_redirect(ex_st);
		cmd->fd_out = -1;
		return (until_pipe_or_newline(lex));
	}
	cmd->fd_out = open(lex[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
	{
		error_open_file(lex[1], ex_st);
		return (until_pipe_or_newline(lex));
	}
	return (2);
}

int	great_great_outfile(char **lex, t_cmd *cmd, int *ex_st)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (lex[1][0] == T_EMPTY)
	{
		error_ambiguous_redirect(ex_st);
		cmd->fd_out = -1;
		return (until_pipe_or_newline(lex));
	}
	cmd->fd_out = open(lex[1], O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (cmd->fd_out == -1)
	{
		error_open_file(lex[1], ex_st);
		return (until_pipe_or_newline(lex));
	}
	return (2);
}

int	is_an_argument(char **lex, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->arg)
	{
		cmd->arg = malloc(sizeof(char *) * (nb_of_arg(lex, 0) + 1));
		if (!cmd->arg)
			return (-1);
		cmd->arg[i] = NULL;
	}
	if (lex[0][0] == T_EMPTY)
		return (1);
	while (cmd->arg[i])
		i++;
	cmd->arg[i] = ft_strdup(lex[0]);
	if (!cmd->arg[i])
		return (-1);
	cmd->arg[i + 1] = NULL;
	return (1);
}
