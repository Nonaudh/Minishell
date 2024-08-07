/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:08:58 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:17:46 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_token(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '\n')
		return (1);
	return (0);
}

int	is_a_token(t_token token)
{
	if (token == GREAT || token == GREATGREAT
		|| token == LESS || token == LESSLESS
		|| token == PIPE || token == T_NEWLINE)
		return (1);
	return (0);
}

int	env_variable_detected(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] != '$' || !ft_isprint(str[i + 1])))
	{
		if (str[i] == '\'')
			i += ft_strchr_index(&str[i + 1], '\'') + 1;
		i++;
	}
	if (str[i])
		return (1);
	return (0);
}

int	not_a_env_var(char *str)
{
	int	i;

	i = 0;
	return (str[i] != '$' || ((!ft_isprint(str[i + 1]) || str[i + 1] == 32)
			|| str[i + 1] == '\"' || str[i + 1] == '\''));
}

int	end_of_env_var(char *str)
{
	int	i;

	i = 1;
	while (ft_isprint(str[i]) && str[i] != 32 && str[i] != '\'' && str[i]
		!= '\"' && str[i - 1] != '?' && str[i] != '$')
		i++;
	return (i);
}
