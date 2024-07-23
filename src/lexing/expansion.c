/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:08:33 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:20:23 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*erase_quote_and_cpy(char *lex, char *str)
{
	int	i;
	int	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			quote = str[i];
			str++;
			while (str[i] && str[i] != quote)
			{
				lex[i] = str[i];
				i++;
			}
			if (str[i])
				str++;
		}
		if (str[i] && str[i] != 34 && str[i] != 39)
		{
			lex[i] = str[i];
			i++;
		}
	}
	lex[i] = 0;
	return (lex);
}

char	*unquote(char *str)
{
	char	*lex;

	if (!str)
		return (NULL);
	lex = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!lex)
		return (NULL);
	lex = erase_quote_and_cpy(lex, str);
	free(str);
	return (lex);
}

char	*add_env_value(char *little, char **env, int *ex_st)
{
	int		x;
	char	*env_value;

	x = 0;
	if (!little)
		return (NULL);
	if (ft_strlen(little) == 1 && little[0] == '?')
	{
		free(little);
		return (ft_itoa(*ex_st));
	}
	env_value = ft_strdup(ft_getenv(little, env));
	free(little);
	if (env_value)
		return (env_value);
	return (ft_strdup("\0"));
}

char	*expand_env(char *str, char **env, int *ex_st)
{
	int		i;
	char	*lex;
	int		start;

	i = 0;
	lex = NULL;
	while (str[i])
	{
		start = i;
		while (str[i] && not_a_env_var(str + i) && str[i] != '\'')
			i++;
		if (str[i] == '\'')
			i += (ft_strchr_index(&str[i + 1], '\'') + 2);
		lex = ft_strjoin_dup_frees(lex, ft_substr(str, start, i - start));
		if (str[i] == '$')
		{
			start = i + 1;
			i += end_of_env_var(str + i);
			lex = ft_strjoin_dup_frees(lex, add_env_value
					(ft_substr(str, start, i - start), env, ex_st));
		}
	}
	free(str);
	return (lex);
}

char	**expand_lex(char **lex, char **env, int *ex_st)
{
	int	i;

	i = 0;
	while (lex[i])
	{
		if (i == 0 || lex[i - 1][0] != LESSLESS)
		{
			if (env_variable_detected(lex[i]))
			{
				lex[i] = expand_env(lex[i], env, ex_st);
				if (!lex[i][0])
				{
					free(lex[i]);
					lex[i] = create_token(T_EMPTY);
				}
			}
			lex[i] = unquote(lex[i]);
			if (!lex[i])
				return (NULL);
		}
		i++;
	}
	return (lex);
}
