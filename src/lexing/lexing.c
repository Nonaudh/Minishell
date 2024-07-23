/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:08:46 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:25:59 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*next_argv(char *line, int *x)
{
	char	*str;
	int		start;

	if (!line[*x])
		return (NULL);
	while (line[*x] && line[*x] <= 32)
		(*x)++;
	if (is_token(line[*x]))
	{
		str = return_token(line, x);
		(*x)++;
		return (str);
	}
	start = *x;
	while (line[*x] > 32 && !is_token(line[*x]))
	{
		if (line[*x] == 34)
			*x += ft_strchr_index(&line[*x + 1], 34) + 1;
		if (line[*x] == 39)
			*x += ft_strchr_index(&line[*x + 1], 39) + 1;
		(*x)++;
	}
	str = ft_substr(line, start, *x - start);
	return (str);
}

int	add_token_newline(char **lex)
{
	int	i;

	i = 0;
	while (lex[i] && lex[i][0])
		i++;
	lex[i] = create_token(T_NEWLINE);
	if (!lex[i])
		return (1);
	return (0);
}

char	**fill_lex(char *line, int argc)
{
	char	**lex;
	int		i;
	int		x;

	i = 0;
	x = 0;
	lex = malloc(sizeof(char *) * (argc + 1));
	if (!lex)
		return (NULL);
	while (i < argc)
	{
		lex[i] = next_argv(line, &x);
		i++;
	}
	if (add_token_newline(lex))
		return (NULL);
	lex[i] = NULL;
	return (lex);
}

int	count_argc(char *str)
{
	int	i;
	int	count;
	int	quote;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] <= 32 && str[i] != '\n')
			i++;
		if (is_token(str[i])
			|| (str[i] > 32 && (str[i + 1] <= 32 || is_token(str[i + 1]))))
			count++;
		if (str[i] == 34 || str[i] == 39)
		{
			quote = ft_strchr_index(&str[i + 1], str[i]);
			if (quote == -1)
				return (error_quote());
			i = i + quote + 1;
			if (str[i + 1] <= 32 || is_token(str[i + 1]))
				count++;
		}
		i++;
	}
	return (count);
}

char	**lexing(char *line, char **env, int *ex_st)
{
	int		argc;
	char	**lex;

	argc = count_argc(line) + 3;
	if (argc < 0)
		return (NULL);
	lex = fill_lex(line, argc);
	if (!lex)
		return (NULL);
	if (check_syntax(lex))
	{
		*ex_st = 2;
		free_the_tab(lex);
		return (NULL);
	}
	lex = expand_lex(lex, env, ex_st);
	return (lex);
}
