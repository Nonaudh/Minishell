#include "../../inc/minishell.h"

int	exec_cmd(t_commands *cmd, t_commands *root, int i)
{
	close_all_fd_except(root, i);
}

int execute_in_child_process(t_commands *cmd, t_commands *root, int i)
{
	int fork_pid;
	int exit_status;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
	{
		exit_status = exec_cmd(cmd, root, i);
		return (exit_status);
	}
	return (0);
}

int execute_command(t_commands *cmd, t_commands *root, int i)
{
	int exit_status;

	exit_status = execute_in_child_process(cmd, root, i);
}

int execution(t_commands *cmd, int size)
{
	int i = 0;
	int exit_status;

	while(i < size)
	{
		exit_status = execute_command(&cmd[i], cmd, i);
		i++;
	}
	return (exit_status);
}
