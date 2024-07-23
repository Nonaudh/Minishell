/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:57:01 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:57:04 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	not_numerical_value(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] && (ft_isdigit(arg[i]) || arg[i] == '+' || arg[i] == '-'))
		i++;
	if (i == 0)
		return (1);
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	if (arg[i])
		return (1);
	return (0);
}

int	positive_exit(int nb)
{
	while (nb > 255)
		nb -= 256;
	return (nb);
}

int	negative_exit(int nb)
{
	while (nb < 0)
		nb += 256;
	return (nb);
}
