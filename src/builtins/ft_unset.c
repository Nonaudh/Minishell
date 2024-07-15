#include "../../inc/minishell.h"

char	**erase_env(char **env_tmp, int i)
{
	int	x;
	char	**env;

	x = 0;
	env = malloc(sizeof(char *) * (count_arg(env_tmp)));
	if (!env)
		return (NULL);
	while (env_tmp[x])
	{
		if (x == i)
			env_tmp++;
		if (env_tmp[x])
		{
			env[x] = ft_strdup(env_tmp[x]);
			if (!env[x])
				return (NULL);
			x++;	
		}
	}
	env[x] = NULL;
	return (env);
}

char	**try_to_unset(t_commands *cmd, int x, int *exit_status)
{
	int	i;
	char **new_env;

	i = 0;
	while (cmd->env[i] && search_arg_in_env(cmd->arg[x], cmd->env[i]))
		i++;
	if (cmd->env[i])
	{
		new_env = erase_env(cmd->env, i);
		free_the_tab(cmd->env);
		return (new_env);
	}
	return (cmd->env);
}

char	**ft_unset(t_commands *cmd, int *exit_status)
{
	int	x;

	x = 1;
	while (cmd->env && cmd->arg[x])
	{
		cmd->env = try_to_unset(cmd, x, exit_status);
		x++;
	}
	return (cmd->env);
}