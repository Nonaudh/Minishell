/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:25 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:27 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

void	error_no_such_file(char *arg, int *ex_st)
{
	*ex_st = 1;
	ft_putstr_fd("minishell: cd: ", 2);
	perror(arg);
}

void	error(char *str, int *ex_st)
{
	*ex_st = 1;
	ft_putstr_fd(str, 2);
}
