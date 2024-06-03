#include "../../inc/minishell.h"

int	is_a_char_token(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '\n')
		return (1);
	return (0);
}

int	is_a_token(t_token token)
{
	if (token == GREAT || token == GREATGREAT
		|| token == LESS || token == LESSLESS
		|| token == PIPE || token == NEWLINE)
		return (1);
	return (0);
}

int	there_is_a_dollar(char *str)
{
	int	i = 0;

	while (str[i] && (str[i] != '$' || !ft_isalnum(str[i + 1])))	
		i++;
	if (str[i])
		return (1);
	return (0);
}