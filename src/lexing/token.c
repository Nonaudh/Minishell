/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:08:53 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:12:17 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	error_token(t_token token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(ft_ttoa(token), 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

int	check_syntax(char **lex)
{
	int	i;

	i = 0;
	if (lex[i][0] == PIPE)
		return (error_token(PIPE));
	while (lex[i + 1]
		&& !(lex[i][0] == PIPE && (lex[i + 1][0] == PIPE
		|| lex[i + 1][0] == T_NEWLINE))
		&& !(lex[i][0] != PIPE && is_a_token(lex[i][0])
		&& is_a_token(lex[i + 1][0])))
		i++;
	if (lex[i + 1])
		return (error_token(lex[i + 1][0]));
	return (0);
}

char	*create_token(t_token token)
{
	char	*str;

	str = malloc(sizeof(char) * 2);
	if (!str)
		return (NULL);
	str[0] = token;
	str[1] = 0;
	return (str);
}

char	*ft_ttoa(t_token token)
{
	if (token == PIPE)
		return ("|");
	if (token == T_NEWLINE)
		return ("newline");
	if (token == GREAT)
		return (">");
	if (token == GREATGREAT)
		return (">>");
	if (token == LESS)
		return ("<");
	if (token == LESSLESS)
		return ("<<");
	return (NULL);
}

char	*return_token(char *str, int *x)
{
	if (str[*x] == '|')
		return (create_token(PIPE));
	if (str[*x] == '\n')
		return (create_token(T_NEWLINE));
	if (str[*x] == '>')
	{
		if (str[*x + 1] == '>')
		{
			(*x)++;
			return (create_token(GREATGREAT));
		}
		return (create_token(GREAT));
	}
	if (str[*x] == '<')
	{
		if (str[*x + 1] == '<')
		{
			(*x)++;
			return (create_token(LESSLESS));
		}
		return (create_token(LESS));
	}
	return (NULL);
}
