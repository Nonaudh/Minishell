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
	int		i;

	i = 0;
	tab = ft_split(ft_getenv("PATH", cmd->env), ':');
	while (tab && tab[i])
	{
		tab[i] = ft_strjoin(tab[i], ft_strjoin("/", cmd->arg[0]));
		i++;
	}
	return (tab);
}

int	is_a_directory(t_commands *cmd, int i, int size)
{
	DIR *dir = NULL;

	dir = opendir(cmd[i].arg[0]);
	if (dir || errno == EACCES)
	{
		printf("bash: %s: Is a directory\n", cmd[i].arg[0]);
		closedir(dir);
		close(cmd[i].fd_in);
		close(cmd[i].fd_out);
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
		while (paths && paths[x])
		{
			execve(paths[x], cmd[i].arg, cmd[i].env);
			x++;
		}
		printf("%s: command not found\n", cmd[i].arg[0]);
		if (paths)
			free_the_tab(paths);
	}
	close(cmd[i].fd_in);
	close(cmd[i].fd_out);
	free_struct_cmd(cmd, size);
	exit(127);
}

int execute_in_child_process(t_commands *cmd, int i, int size)
{
	int fork_pid;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
		exec_cmd(cmd, i, size);
	return (0);
}

int execute_command(t_commands *cmd, int i, int size)
{
	int exit_status;
	int tmp_stdin;
	int tmp_stdout;

	tmp_stdin = dup(STDIN_FILENO);
	tmp_stdout = dup(STDOUT_FILENO);

	dup2(cmd[i].fd_in, STDIN_FILENO);
	dup2(cmd[i].fd_out, STDOUT_FILENO);

	execute_in_child_process(cmd, i, size);

	dup2(tmp_stdin, STDIN_FILENO);
	dup2(tmp_stdout, STDOUT_FILENO);
	close(tmp_stdin);
	close (tmp_stdout);
	return (0);
}

int	wait_for_all_process(void)
{
	int status;

	while (waitpid(-1, &status, 0) > 0)
		;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int execution(t_commands *cmd, int size)
{
	int i = 0;
	int exit_status;

	while(i < size)
	{
		if (cmd[i].fd_in == -1 || cmd[i].fd_out == -1)
			exit_status = 1;
		else
			execute_command(cmd, i, size);
		i++;
	}
	close_all_fd_except(cmd, -1, size);
	exit_status = wait_for_all_process();
	return (exit_status);
}
