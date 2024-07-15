/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:36:08 by bdany             #+#    #+#             */
/*   Updated: 2024/07/01 18:57:33 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	count_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		i++;
	}
	return (i);
}

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
	old_pwd = ft_strjoin(str, pwd);
	free(str);
	return (old_pwd);
}

char	**put_pwd_in_old_pwd(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "OLDPWD=", 7))
		i++;
	if (env[i])
	{
		env[i] = troncate_equal(env[i]);
		env[i] = join_with_pwd(env, env[i]);
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
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4))
		i++;
	if (env[i])
	{
		env[i] = troncate_equal(env[i]);
		env[i] = join_with_new_pwd(env[i]);
	}
	return (env);
}

char	**switch_pwd_env(char **env)
{
	env = put_pwd_in_old_pwd(env);
	env = update_pwd(env);
	return (env);
}

char	**ft_cd(t_commands *cmd, int *exit_status)
{
	char	*home;

	if (count_arg(&cmd->arg[1]) == 1)
	{
		if (chdir(cmd->arg[1]))
			printf("minishell: cd: %s: No such file or directory\n", cmd->arg[1]);
		else
			cmd->env = switch_pwd_env(cmd->env);
	}
	else if (count_arg(&cmd->arg[1]) == 0)
	{
		home = ft_getenv("HOME", cmd->env);
		if (!home)
			printf("bash: cd: HOME not set\n");
		else
		{
			chdir(home);
			cmd->env = switch_pwd_env(cmd->env);
		}
	}
	else
		printf("bash: cd: too many arguments\n");
	return (cmd->env);
}

