/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:43 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:45 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_argument_env(char **arg)
{
	if (arg[1])
		return (1);
	return (0);
}

char	**error_argument(t_cmd *cmd, int *ex_st, int error_code)
{
	*ex_st = error_code;
	ft_putstr_fd("bash: env: ", 2);
	ft_putstr_fd(cmd->arg[1], 2);
	ft_putstr_fd(": invalid argument\n", 2);
	return (cmd->env);
}

int	ft_env(t_cmd *cmd, int *ex_st)
{
	int	i;

	i = 0;
	*ex_st = 0;
	if (check_option(cmd->arg))
	{
		error_option(cmd, ex_st, 125);
		return (0);
	}
	if (check_argument_env(cmd->arg))
	{
		error_argument(cmd, ex_st, 127);
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
