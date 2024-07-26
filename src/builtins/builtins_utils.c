/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:55:34 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:55:37 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_option(char **arg)
{
	if (arg[1] && arg[1][0] == '-' && arg[1][1])
		return (1);
	return (0);
}

char	**error_option(t_cmd *cmd, int *ex_st, int error_code)
{
	*ex_st = error_code;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->arg[0], 2);
	ft_putstr_fd(": ", 2);
	write(2, cmd->arg[1], 2);
	ft_putstr_fd(": invalid option\n", 2);
	return (cmd->env);
}
