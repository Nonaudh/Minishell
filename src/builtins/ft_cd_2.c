/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:18 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:20 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*troncate_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i])
	{
		str[i + 1] = 0;
		return (str);
	}
	return (NULL);
}

char	*join_with_pwd(char **env, char *str)
{
	char	*old_pwd;
	char	*pwd;

	pwd = ft_getenv("PWD", env);
	if (!pwd)
		return (str);
	old_pwd = ft_strjoin(str, pwd);
	free(str);
	return (old_pwd);
}

char	**put_pwd_in_old_pwd(char **env)
{
	int		i;
	char	*str;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "OLDPWD=", 7))
		i++;
	if (env[i])
	{
		env[i] = troncate_equal(env[i]);
		env[i] = join_with_pwd(env, env[i]);
		if (!env[i])
			return (NULL);
	}
	else
	{
		str = ft_getenv("PWD", env);
		if (str)
		{
			str = ft_strjoin("OLDPWD=", str);
			env = create_arg(str, env);
			free(str);
		}
	}
	return (env);
}

char	*join_with_new_pwd(char *str)
{
	char	cwd[1024];
	char	*new_pwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		new_pwd = ft_strjoin(str, cwd);
		free(str);
		return (new_pwd);
	}
	return (NULL);
}

char	**update_pwd(char **env)
{
	int		i;
	char	*str;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4))
		i++;
	if (env[i])
	{
		env[i] = troncate_equal(env[i]);
		env[i] = join_with_new_pwd(env[i]);
		if (!env[i])
			return (NULL);
	}
	else
	{
		str = join_with_new_pwd(ft_strdup("PWD="));
		env = create_arg(str, env);
		free(str);
	}
	return (env);
}
