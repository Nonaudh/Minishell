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

void	error_is_a_directory(char *dir)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

int	is_a_directory(t_commands *cmd, int i, int size)
{
	DIR *dir = NULL;

	dir = opendir(cmd[i].arg[0]);
	if (dir)
	{
		error_is_a_directory(cmd[i].arg[0]);
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

void	error_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	exec_cmd(t_commands *cmd, int i, int size)
{
	char **paths;
	int x = 0;

	close_all_fd_except(cmd, i, size);
	execve(cmd[i].arg[0], cmd[i].arg, cmd[i].env);
	if (ft_strchr(cmd[i].arg[0], '/'))
		is_a_directory(cmd, i, size);
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

char	**execute_in_main_process(t_commands *cmd, int *exit_status)
{
	char **env = cmd->env;
	if (!strcmp(cmd->arg[0], "echo"))
		ft_echo(cmd, exit_status);
	else if (!strcmp(cmd->arg[0], "cd"))
		env = ft_cd(cmd, exit_status);
	else if (!strcmp(cmd->arg[0], "pwd"))
		ft_pwd();
	else if (!strcmp(cmd->arg[0], "export"))
		env = ft_export(cmd, exit_status);
	else if (!strcmp(cmd->arg[0], "unset"))
		env = ft_unset(cmd, exit_status);
	else if (!strcmp(cmd->arg[0], "env"))
		ft_env(cmd, exit_status);
	// else if (!strcmp(arg[0], "exit"))
	// 	;
	return (env);
}

int	is_a_buildins(char **arg)
{
	if (!strcmp(arg[0], "echo") || !strcmp(arg[0], "cd")
		|| !strcmp(arg[0], "pwd") || !strcmp(arg[0], "export")
		|| !strcmp(arg[0], "unset") ||!strcmp(arg[0], "env")
		|| !strcmp(arg[0], "exit"))
		return (1);
	return (0);
}

char	**execute_builtins(t_commands *cmd, int *exit_status)
{
	int		fd_tmp[2];
	char	**env;

	fd_tmp[0] = dup(STDIN_FILENO);
	fd_tmp[1] =  dup(STDOUT_FILENO);
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);

	env = execute_in_main_process(cmd, exit_status);

	dup2(fd_tmp[0], STDIN_FILENO);
	dup2(fd_tmp[1], STDOUT_FILENO);
	close(fd_tmp[0]);
	close (fd_tmp[1]);
	return (env);
}

int	execute_command(t_commands *cmd, int i, int size, int *exit_status)
{
	int fork_pid;
	int	fd_tmp[2];

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
	int status = 0;
	if (fork_pid != 42)
		waitpid(fork_pid, &status, 0);
	if (fork_pid != 42 && WIFEXITED(status))
	{
		*exit_status = WEXITSTATUS(status);
	}
	else if (fork_pid != 42 && g_sig_flag)
	{
		*exit_status = 69;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	g_sig_flag = 0;
	return (0);
}

char	**execution(t_commands *cmd, int size, int *exit_status)
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
			if (is_a_buildins(cmd[i].arg))
			{
				cmd->env = execute_builtins(&cmd[i], exit_status);
				fork_pid = -42;
			}
			else
				fork_pid = execute_command(cmd, i, size, exit_status);
		}
		i++;
	}
	close_all_fd_except(cmd, -1, size);
	wait_for_all_process(fork_pid, exit_status);
	return (cmd->env);
}
