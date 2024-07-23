/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:21:13 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 15:21:48 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	close_all_fd_except(t_cmd *cmd, int i, int size)
{
	int	x;

	x = 0;
	while (x < size)
	{
		if (x != i)
		{
			if (cmd[x].fd_in != STDIN_FILENO && cmd[x].fd_in != -1)
			{
				if (close(cmd[x].fd_in))
					perror("fd_in");
			}
			if (cmd[x].fd_out != STDOUT_FILENO && cmd[x].fd_out != -1)
			{
				if (close(cmd[x].fd_out))
					perror("fd_out");
			}
		}
		x++;
	}
	return (0);
}

int	wait_for_all_process(int fork_pid, int *ex_st)
{
	int	status;

	status = 0;
	if (fork_pid != -42)
	{
		waitpid(fork_pid, &status, 0);
		if (WIFEXITED(status))
			*ex_st = WEXITSTATUS(status);
	}
	if (g_sig_flag)
		*ex_st = g_sig_flag;
	while (waitpid(-1, NULL, 0) > 0)
		;
	g_sig_flag = 0;
	return (0);
}

char	**execution(t_cmd *cmd, int size, int *ex_st)
{
	int	i;
	int	fork_pid;

	i = 0;
	fork_pid = -42;
	while (i < size)
	{
		if (cmd[i].fd_in != -1 && cmd[i].fd_out != -1)
		{
			if (is_a_buildins(cmd[i].arg))
				cmd->env = execute_builtins(&cmd[i], size, ex_st, &fork_pid);
			else
				fork_pid = execute_command(cmd, i, size, ex_st);
		}
		i++;
	}
	close_all_fd_except(cmd, -1, size);
	wait_for_all_process(fork_pid, ex_st);
	return (cmd->env);
}
