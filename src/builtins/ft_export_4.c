/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:56:04 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:56:05 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	error_arg_export(char *str, int *ex_st)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*ex_st = 1;
	return (1);
}

int	check_argument_export(char *str, int *ex_st)
{
	int	i;

	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (error_arg_export(str, ex_st));
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (error_arg_export(str, ex_st));
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (error_arg_export(str, ex_st));
	return (0);
}

int	search_arg_in_env(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+' && env[i] && env[i] != '=')
	{
		if (str[i] != env[i])
			return (1);
		i++;
	}
	if ((!str[i] || str[i] == '=' || str[i] == '+')
		&& (!env[i] || env[i] == '='))
		return (0);
	return (1);
}
