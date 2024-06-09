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
		|| token == PIPE || token == T_NEWLINE)
		return (1);
	return (0);
}

int	env_variable_detected(char *str)
{
	int	i = 0;

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