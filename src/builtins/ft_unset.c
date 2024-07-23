/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:57:29 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:57:31 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**erase_env(char **env_tmp, int i)
{
	int		x;
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

char	**try_to_unset(t_cmd *cmd, int x)
{
	int		i;
	char	**new_env;

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

char	**ft_unset(t_cmd *cmd, int size, int *ex_st)
{
	int	x;

	x = 1;
	*ex_st = 0;
	if (check_option(cmd->arg))
		return (error_option(cmd, ex_st, 2));
	if (size > 1)
		return (cmd->env);
	while (cmd->env && cmd->arg[x])
	{
		cmd->env = try_to_unset(cmd, x);
		x++;
	}
	return (cmd->env);
}
