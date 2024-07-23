/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:52 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:53 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_value(char *arg)
{
	int	nb;

	nb = ft_atol(arg);
	if (nb > 0)
		return (positive_exit(nb));
	return (negative_exit(nb));
}

int	return_value_exit(char *arg, int *ex_st)
{
	if (!arg)
		return (0);
	else if (not_numerical_value(arg))
	{
		*ex_st = 2;
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (1);
	}
	else
		*ex_st = exit_value(arg);
	return (0);
}

int	check_arg_exit(char **arg, int *ex_st)
{

	if (return_value_exit(arg[0], ex_st))
		return (1);
	if (arg[0] && arg[1])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		*ex_st = 1;
		return (0);
	}
	return (1);
}

char	**ft_exit(t_cmd *cmd, int *ex_st)
{
	if (check_arg_exit(cmd->arg + 1, ex_st))
		return (NULL);
	return (cmd->env);
}
