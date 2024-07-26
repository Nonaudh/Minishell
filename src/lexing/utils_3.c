/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:09:04 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:23:57 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	error_quote(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected unclosed quote\n", 2);
	return (-42);
}

void	free_the_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ugly_copy_plus_plus(char *src, char *dst, int *i)
{
	dst[0] = src[0];
	(*i)++;
}

int	count_plus_plus(char *str)
{
	int	i;

	i = 0;
	return (str[i] && (is_token(str[i])
			|| (str[i] > 32 && (str[i + 1] <= 32 || is_token(str[i + 1])))));
}
