/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:25:29 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 15:27:45 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**create_paths_tab(t_cmd *cmd)
{
	char	**tab;
	char	*cmd_path;
	char	*new_tab;
	int		i;

	i = 0;
	tab = ft_split(ft_getenv("PATH", cmd->env), ':');
	while (tab && tab[i])
	{
		cmd_path = ft_strjoin("/", cmd->arg[0]);
		if (!cmd_path)
			return (NULL);
		new_tab = ft_strjoin(tab[i], cmd_path);
		if (!new_tab)
			return (NULL);
		free(cmd_path);
		free(tab[i]);
		tab[i] = new_tab;
		i++;
	}
	return (tab);
}

int	is_a_directory(t_cmd *cmd, int i, int size)
{
	DIR	*dir;

	dir = NULL;
	dir = opendir(cmd[i].arg[0]);
	if (dir || errno == EACCES)
	{
		error_is_a_directory(cmd[i].arg[0]);
		closedir(dir);
		close(cmd[i].fd_in);
		close(cmd[i].fd_out);
		free_the_tab(cmd[i].env);
		free_struct_cmd(cmd, size);
		exit(126);
	}
	if (access(cmd[i].arg[0], X_OK))
		perror(cmd[i].arg[0]);
	if (errno == EACCES)
		return (126);
	return (127);
}

int	exec_cmd(t_cmd *cmd, int i, int size)
{
	char	**paths;
	int		x;
	int		error;

	x = 0;
	error = 127;
	close_all_fd_except(cmd, -1, size);
	execve(cmd[i].arg[0], cmd[i].arg, cmd[i].env);
	if (ft_strchr(cmd[i].arg[0], '/'))
		error = is_a_directory(cmd, i, size);
	else
	{
		paths = create_paths_tab(&cmd[i]);
		while (paths && paths[x] && execve(paths[x], cmd[i].arg, cmd[i].env))
			x++;
		error_cmd_not_found(cmd[i].arg[0]);
		if (paths)
			free_the_tab(paths);
	}
	close(cmd[i].fd_in);
	close(cmd[i].fd_out);
	free_the_tab(cmd[i].env);
	free_struct_cmd(cmd, size);
	exit(error);
}

int	execute_in_child_process(t_cmd *cmd, int i, int size)
{
	int	fork_pid;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
	{
		dup2(cmd[i].fd_in, STDIN_FILENO);
		dup2(cmd[i].fd_out, STDOUT_FILENO);
		exec_cmd(cmd, i, size);
	}
	return (fork_pid);
}

int	execute_command(t_cmd *cmd, int i, int size, int *ex_st)
{
	int	fork_pid;

	if (!cmd[i].arg || !cmd[i].arg[0])
	{
		*ex_st = 0;
		return (-42);
	}
	fork_pid = execute_in_child_process(cmd, i, size);
	return (fork_pid);
}
