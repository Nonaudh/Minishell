#include "../../inc/minishell.h"

int	close_all_fd_except(t_commands *cmd, int i, int size)
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

char	**create_paths_tab(t_commands *cmd)
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

int	is_a_directory(t_commands *cmd, int i, int size)
{
	DIR *dir = NULL;

	dir = opendir(cmd[i].arg[0]);
	if (dir)
	{
		printf("bash: %s: Is a directory\n", cmd[i].arg[0]);
		closedir(dir);
		close(cmd[i].fd_in);
		close(cmd[i].fd_out);
		free_the_tab(cmd[i].env);
		free_struct_cmd(cmd, size);
		exit(126);
	}
	ft_putstr_fd("bash: ", 2);
	perror(cmd[i].arg[0]);
	return (0);
}

int	exec_cmd(t_commands *cmd, int i, int size)
{
	char **paths;
	int x = 0;

	close_all_fd_except(cmd, i, size);
	execve(cmd[i].arg[0], cmd[i].arg, cmd[i].env);
	if (strchr(cmd[i].arg[0], '/'))
		is_a_directory(cmd, i, size);
	else
	{
		paths = create_paths_tab(&cmd[i]);
		while (paths && paths[x] && execve(paths[x], cmd[i].arg, cmd[i].env))
			x++;
		printf("%s: command not found\n", cmd[i].arg[0]);
		if (paths)
			free_the_tab(paths);
	}
	close(cmd[i].fd_in);
	close(cmd[i].fd_out);
	free_the_tab(cmd[i].env);
	free_struct_cmd(cmd, size);
	exit(127);
}

int execute_in_child_process(t_commands *cmd, int i, int size, int *fd_tmp)
{
	int fork_pid;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
	{
		close(fd_tmp[0]);
		close(fd_tmp[1]);
		exec_cmd(cmd, i, size);
	}
	return (fork_pid);
}

int execute_command(t_commands *cmd, int i, int size)
{
	int exit_status;
	int	fd_tmp[2];
	int fork_pid;

	fd_tmp[0] = dup(STDIN_FILENO);
	fd_tmp[1] =  dup(STDOUT_FILENO);
	dup2(cmd[i].fd_in, STDIN_FILENO);
	dup2(cmd[i].fd_out, STDOUT_FILENO);

	fork_pid = execute_in_child_process(cmd, i, size, fd_tmp);

	dup2(fd_tmp[0], STDIN_FILENO);
	dup2(fd_tmp[1], STDOUT_FILENO);
	close(fd_tmp[0]);
	close (fd_tmp[1]);
	return (fork_pid);
}

int	wait_for_all_process(int fork_pid, int *exit_status)
{
	int status;

	waitpid(fork_pid, &status, 0);
	if (WIFEXITED(status))
	{
		*exit_status = WEXITSTATUS(status);
	}
	else if (g_sig_flag)
	{
		*exit_status = 69;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	g_sig_flag = 0;
	printf("exit; %d\n", *exit_status);
	return(*exit_status);
}

int execution(t_commands *cmd, int size, int *exit_status)
{
	int i = 0;
	int fork_pid;

	fork_pid = 0;
	while(i < size)
	{
		if (cmd[i].fd_in == -1 || cmd[i].fd_out == -1)
			*exit_status = 1;
		else if (!cmd[i].arg[0])
			*exit_status = 0;
		else
		{
			fork_pid = execute_command(cmd, i, size);
		}
		i++;
	}
	wait_for_all_process(fork_pid, exit_status);
	close_all_fd_except(cmd, -1, size);
	return (*exit_status);
}
