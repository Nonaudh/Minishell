#include "../../inc/minishell.h"

int	ft_env(t_commands *cmd, int *exit_status)
{
	int	i;

	i = 0;
	while (cmd->env[i])
	{
		if (ft_strchr(cmd->env[i], '='))		
			ft_putendl_fd(cmd->env[i], 1);
		i++;
	}
	return (0);
}