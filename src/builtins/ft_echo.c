/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:33 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:35 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_n_option(char *arg)
{
	int	i;

	i = 0;
	if (arg && !ft_strncmp(arg, "-n", 2))
	{
		i++;
		while (arg[i] && arg[i] == 'n')
			i++;
		if (!arg[i])
			return (1);
	}
	return (0);
}

void	print_without_new(char **arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i + 1])
	{
		ft_putstr_fd(arg[i], 1);
		ft_putstr_fd(" ", 1);
		i++;
	}
	if (arg[i])
		ft_putstr_fd(arg[i], 1);
}

void	print_with_new(char **arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i + 1])
	{
		ft_putstr_fd(arg[i], 1);
		ft_putstr_fd(" ", 1);
		i++;
	}
	if (arg[i])
		ft_putstr_fd(arg[i], 1);
	ft_putstr_fd("\n", 1);
}

int	ft_echo(t_cmd *cmd, int *ex_st)
{
	int	i;

	i = 1;
	*ex_st = 0;
	while (cmd->arg[i] && is_n_option(cmd->arg[i]))
		i++;
	if (i != 1)
		print_without_new(&cmd->arg[i]);
	else
		print_with_new(&cmd->arg[i]);
	return (0);
}
