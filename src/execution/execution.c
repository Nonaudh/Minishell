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

void	error_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	print_fd(t_commands *cmd)
{
	ft_putendl_fd(ft_itoa(cmd->fd_in), 2);
	ft_putendl_fd(ft_itoa(cmd->fd_out), 2);
}

int	exec_cmd(t_commands *cmd, int i, int size)
{
	char **paths;
	int x = 0;
	int error = 127;

	close_all_fd_except(cmd, i, size);

	print_fd(&cmd[i]);

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

int execute_in_child_process(t_commands *cmd, int i, int size, int *fd_tmp)
{
	int fork_pid;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
	{
		dup2(cmd[i].fd_in, STDIN_FILENO);
		dup2(cmd[i].fd_out, STDOUT_FILENO);
		// close(fd_tmp[0]);
		// close(fd_tmp[1]);
		exec_cmd(cmd, i, size);
	}
	return (fork_pid);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	**execute_in_main_process(t_commands *cmd, int size, int *exit_status)
{
	char **env;

	env = cmd->env;
	if (!ft_strcmp(cmd->arg[0], "echo"))
		ft_echo(cmd, exit_status);
	else if (!strcmp(cmd->arg[0], "cd"))
		env = ft_cd(cmd, size, exit_status);
	else if (!ft_strcmp(cmd->arg[0], "pwd"))
		ft_pwd(cmd, exit_status);
	else if (!ft_strcmp(cmd->arg[0], "export"))
		env = ft_export(cmd, size, exit_status);
	else if (!ft_strcmp(cmd->arg[0], "unset"))
		env = ft_unset(cmd, size, exit_status);
	else if (!ft_strcmp(cmd->arg[0], "env"))
		ft_env(cmd, exit_status);
	else if (size == 1 && !strcmp(cmd->arg[0], "exit"))
		env = ft_exit(cmd, exit_status);
	if (env != cmd->env)
		free_the_tab(cmd->env);
	return (env);
}

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

char	**execute_builtins(t_commands *cmd, int size, int *exit_status, int *fork_pid)
{
	int		fd_tmp[2];
	char	**env;

	*fork_pid = -42;
	fd_tmp[0] = dup(STDIN_FILENO);
	fd_tmp[1] =  dup(STDOUT_FILENO);
	dup2(cmd->fd_in, STDIN_FILENO);
	dup2(cmd->fd_out, STDOUT_FILENO);

	env = execute_in_main_process(cmd, size, exit_status);

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

	if (!cmd[i].arg || !cmd[i].arg[0])
	{
		*exit_status = 0;
		return (-42);
	}
	// fd_tmp[0] = dup(STDIN_FILENO);
	// fd_tmp[1] =  dup(STDOUT_FILENO);
	// dup2(cmd[i].fd_in, STDIN_FILENO);
	// dup2(cmd[i].fd_out, STDOUT_FILENO);

	fork_pid = execute_in_child_process(cmd, i, size, fd_tmp);

	// dup2(fd_tmp[0], STDIN_FILENO);
	// dup2(fd_tmp[1], STDOUT_FILENO);
	// close(fd_tmp[0]);
	// close (fd_tmp[1]);
	return (fork_pid);
}

int	wait_for_all_process(t_commands *cmd, int *fork_pid, int size, int *exit_status)
{
	int status;
	int	i;

	i = 0;
	status = 0;
	if (fork_pid[size - 1] != -42)
	{
		waitpid(fork_pid[size - 1], &status, 0);
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
	}
	if (g_sig_flag)
		*exit_status = g_sig_flag;

	while (i < (size - 1))
	{
		if (fork_pid[i] != -42)
			waitpid(fork_pid[i], NULL, 0);
		if (cmd[i].fd_in != STDIN_FILENO && cmd[i].fd_in != -1)
			close(cmd[i].fd_in);
		if (cmd[i].fd_out != STDOUT_FILENO && cmd[i].fd_out != -1)
			close(cmd[i].fd_out);
		i++;
	}
	if (cmd[i].fd_in != STDIN_FILENO && cmd[i].fd_in != -1)
		close(cmd[size].fd_in);
	if (cmd[i].fd_out != STDOUT_FILENO && cmd[i].fd_out != -1)
		close(cmd[size].fd_out);

	// while (waitpid(-1, NULL, 0) > 0)
	// 	;
	g_sig_flag = 0;
	return (0);
}

char	**execution(t_commands *cmd, int size, int *exit_status)
{
	int i;
	int *fork_pid;

	i = 0;
	fork_pid = malloc(sizeof(int) * size);
	ft_memset(fork_pid, -42, size);
	while(i < size)
	{
		if (cmd[i].fd_in != -1 && cmd[i].fd_out != -1)
		{
			if (is_a_buildins(cmd[i].arg))
				cmd->env = execute_builtins(&cmd[i], size, exit_status, &fork_pid[i]);
			else
				fork_pid[i] = execute_command(cmd, i, size, exit_status);
		}
		else
		{
			*exit_status = 1;
			fork_pid[i] = -42;
		}
		i++;
	}
	//close_all_fd_except(cmd, -1, size);
	wait_for_all_process(cmd, fork_pid, size, exit_status);
	return (cmd->env);
}
