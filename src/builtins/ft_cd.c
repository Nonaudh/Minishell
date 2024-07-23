/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:36:08 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:13 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**switch_pwd_env(char **env)
{
	env = put_pwd_in_old_pwd(env);
	env = update_pwd(env);
	return (env);
}

char	**cd_to_dir(t_cmd *cmd, int *ex_st)
{
	if (chdir(cmd->arg[1]))
		error_no_such_file(cmd->arg[1], ex_st);
	else
		cmd->env = switch_pwd_env(cmd->env);
	return (cmd->env);
}

char	**cd_to_root(t_cmd *cmd, int *ex_st)
{
	char	*home;

	home = ft_getenv("HOME", cmd->env);
	if (!home)
		error("bash: cd: HOME not set\n", ex_st);
	else
	{
		if (chdir(home))
			error_no_such_file(home, ex_st);
		else
			cmd->env = switch_pwd_env(cmd->env);
	}
	return (cmd->env);
}

char	**ft_cd(t_cmd *cmd, int size, int *ex_st)
{
	*ex_st = 0;
	if (check_option(cmd->arg))
		return (error_option(cmd, ex_st, 2));
	if (size > 1)
		return (cmd->env);
	if (count_arg(&cmd->arg[1]) == 1)
		cmd->env = cd_to_dir(cmd, ex_st);
	else if (count_arg(&cmd->arg[1]) == 0)
		cmd->env = cd_to_root(cmd, ex_st);
	else
		error("bash: cd: too many arguments\n", ex_st);
	return (cmd->env);
}
