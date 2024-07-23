/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:22:51 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 15:28:08 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_a_buildins(char **arg)
{
	if (!arg || !arg[0])
		return (0);
	if (!ft_strcmp(arg[0], "echo") || !ft_strcmp(arg[0], "cd")
		|| !ft_strcmp(arg[0], "pwd") || !ft_strcmp(arg[0], "export")
		|| !ft_strcmp(arg[0], "unset") ||!ft_strcmp(arg[0], "env")
		|| !ft_strcmp(arg[0], "exit"))
		return (1);
	return (0);
}

char	**execute_in_main_process(t_cmd *cmd, int size, int *ex_st)
{
	char	**env;

	env = cmd->env;
	if (!ft_strcmp(cmd->arg[0], "echo"))
		ft_echo(cmd, ex_st);
	else if (!strcmp(cmd->arg[0], "cd"))
		env = ft_cd(cmd, size, ex_st);
	else if (!ft_strcmp(cmd->arg[0], "pwd"))
		ft_pwd(cmd, ex_st);
	else if (!ft_strcmp(cmd->arg[0], "export"))
		env = ft_export(cmd, size, ex_st);
	else if (!ft_strcmp(cmd->arg[0], "unset"))
		env = ft_unset(cmd, size, ex_st);
	else if (!ft_strcmp(cmd->arg[0], "env"))
		ft_env(cmd, ex_st);
	else if (size == 1 && !strcmp(cmd->arg[0], "exit"))
		env = ft_exit(cmd, ex_st);
	if (env != cmd->env)
		free_the_tab(cmd->env);
	return (env);
}

void	ignore_sigpipe(void)
{
	struct sigaction	sa2;

	ft_bzero(&sa2, sizeof(struct sigaction));
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa2, NULL);
}

void	default_sigpipe(void)
{
	struct sigaction	sa2;

	ft_bzero(&sa2, sizeof(struct sigaction));
	sa2.sa_handler = SIG_DFL;
	sigaction(SIGPIPE, &sa2, NULL);
}

char	**execute_builtins(t_cmd *cmd, int size, int *ex_st, int *fork_pid)
{
	int		fd_tmp[2];
	char	**env;

	*fork_pid = -42;
	fd_tmp[0] = dup(STDIN_FILENO);
	fd_tmp[1] = dup(STDOUT_FILENO);
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);
	ignore_sigpipe();
	env = execute_in_main_process(cmd, size, ex_st);
	default_sigpipe();
	dup2(fd_tmp[0], STDIN_FILENO);
	dup2(fd_tmp[1], STDOUT_FILENO);
	close(fd_tmp[0]);
	close (fd_tmp[1]);
	return (env);
}
