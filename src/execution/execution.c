#include "../../inc/minishell.h"

int	close_all_fd_except(t_commands *root, int i, int size)
{
	int	x;

	x = 0;
	while (x < size)
	{
		if (x != i)
		{
			if (close(root->fd_in))
				printf("Error close fd_in\n");
			if (close(root->fd_out))
				printf("Error close fd_out\n");			
		}
		x++;
	}
	return (0);
}

int	exec_cmd(t_commands *cmd, t_commands *root, int i, int size)
{
	close_all_fd_except(root, i, size);
	
}

int execute_in_child_process(t_commands *cmd, t_commands *root, int i, int size)
{
	int fork_pid;
	int exit_status;

	fork_pid = fork();
	if (fork_pid < 0)
		exit (EXIT_FAILURE);
	if (fork_pid == 0)
	{
		exit_status = exec_cmd(cmd, root, i, size);
		return (exit_status);
	}
	return (0);
}

int execute_command(t_commands *cmd, t_commands *root, int i, int size)
{
	int exit_status;

	exit_status = execute_in_child_process(cmd, root, i, size);
}

int execution(t_commands *cmd, int size)
{
	int i = 0;
	int exit_status;

	while(i < size)
	{
		exit_status = execute_command(&cmd[i], cmd, i, size);
		i++;
	}
	return (exit_status);
}
