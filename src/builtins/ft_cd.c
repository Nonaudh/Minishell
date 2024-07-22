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
	}
	return (env);
}

char	**switch_pwd_env(char **env)
{
	env = put_pwd_in_old_pwd(env);
	env = update_pwd(env);
	return (env);
}

void	error_no_such_file(char *arg, int *exit_status)
{
	*exit_status = 1;
	ft_putstr_fd("minishell: cd: ", 2);
	// ft_putstr_fd(arg, 2);
	// ft_putstr_fd(": No such file or directory\n", 2);
	perror(arg);
}

void	error(char *str, int *exit_status)
{
	*exit_status = 1;
	ft_putstr_fd(str, 2);
}

char	**ft_cd(t_commands *cmd, int size, int *exit_status)
{
	char	*home;

	*exit_status = 0;
	if (check_option(cmd->arg))
		return (error_option(cmd, exit_status, 2));
	if (size > 1)
		return (cmd->env);
	if (count_arg(&cmd->arg[1]) == 1)
	{
		if (chdir(cmd->arg[1]))
			error_no_such_file(cmd->arg[1], exit_status);
		else
			cmd->env = switch_pwd_env(cmd->env);
	}
	else if (count_arg(&cmd->arg[1]) == 0)
	{
		home = ft_getenv("HOME", cmd->env);
		if (!home)
			error("bash: cd: HOME not set\n", exit_status);
		else
		{
			if (chdir(home))
				error_no_such_file(home, exit_status);
			cmd->env = switch_pwd_env(cmd->env);
		}
	}
	else
		error("bash: cd: too many arguments\n", exit_status);
	return (cmd->env);
}

