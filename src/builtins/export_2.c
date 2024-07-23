/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:55:44 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:55:53 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**realloc_and_copy_env(char **env_tmp, int size)
{
	char	**env;
	int		i;

	i = 0;
	env = malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (NULL);
	while (env_tmp[i])
	{
		env[i] = ft_strdup(env_tmp[i]);
		if (!env[i])
			return (NULL);
		i++;
	}
	env[i] = NULL;
	free_the_tab(env_tmp);
	return (env);
}

char	*create_without_plus(char *str)
{
	char	*env;
	int		i;

	i = 0;
	env = malloc(sizeof(char) * ft_strlen(str));
	if (!env)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '+')
			str++;
		env[i] = str[i];
		i++;
	}
	env[i] = 0;
	return (env);
}

char	**create_arg(char *str, char **env_tmp)
{
	int		i;
	char	**env;
	int		size_env;

	size_env = count_arg(env_tmp);
	env = realloc_and_copy_env(env_tmp, size_env + 1);
	if (!env)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	if (!str[i] || str[i] == '=')
		env[size_env] = ft_strdup(str);
	else
		env[size_env] = create_without_plus(str);
	if (!env[size_env])
		return (NULL);
	env[size_env + 1] = NULL;
	return (env);
}

char	**add_arg_to_env(char *str, char **env, int size, int *ex_st)
{
	int	i;

	i = 0;
	if (check_argument_export(str, ex_st) || size > 1)
		return (env);
	while (env[i] && search_arg_in_env(str, env[i]))
		i++;
	if (env[i])
	{
		env[i] = replace_with_arg(str, env[i]);
		if (!env[i])
			return (NULL);
	}
	else
		env = create_arg(str, env);
	return (env);
}

char	**add_env_variable(t_cmd *cmd, int size, int *ex_st)
{
	int		i;

	i = 1;
	if (check_option(cmd->arg))
		return (error_option(cmd, ex_st, 2));
	while (cmd->env && cmd->arg[i])
	{
		cmd->env = add_arg_to_env(cmd->arg[i], cmd->env, size, ex_st);
		i++;
	}
	return (cmd->env);
}
