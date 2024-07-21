#include "../../inc/minishell.h"

int	check_argument_env(char **arg)
{
	if (arg[1])
		return (1);
}

char	**error_argument(t_commands *cmd, int *exit_status, int error_code)
{
	*exit_status = error_code;	
	ft_putstr_fd("bash: env: ", 2);
	ft_putstr_fd(cmd->arg[1], 2);
	ft_putstr_fd(": invalid argument\n", 2);
	return (cmd->env);
}

int	ft_env(t_commands *cmd, int *exit_status)
{
	int	i;

	i = 0;
	*exit_status = 0;
	if (check_option(cmd->arg))
	{
		error_option(cmd, exit_status, 125);
		return (0);
	}
	if (check_argument_env(cmd->arg))
	{
		error_argument(cmd, exit_status, 127);
		return (0);
	}
	while (cmd->env[i])
	{
		if (ft_strchr(cmd->env[i], '='))		
			ft_putendl_fd(cmd->env[i], 1);
		i++;
	}
	return (0);
}
