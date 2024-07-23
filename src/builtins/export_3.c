/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:55:58 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:00 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*replace(char *str, char *env)
{
	free(env);
	env = ft_strdup(str);
	return (env);
}

char	*join_arg(char *str, char *env)
{
	char	*new_env;

	new_env = ft_strjoin(env, str);
	free(env);
	return (new_env);
}

char	*replace_with_arg(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	if (!str[i])
		return (env);
	if (str[i] == '=')
		env = replace(str, env);
	if (str[i] == '+')
		env = join_arg(str + i + 2, env);
	return (env);
}
