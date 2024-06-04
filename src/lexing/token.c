#include "../../inc/minishell.h"

int	error_token(t_token token)
{
	printf("bash: syntax error near unexpected token `%s'\n", ft_ttoa(token));
	return (1);
}

int	check_syntax(char **lex)
{
	int	i = 0;

	if (lex[i][0] == PIPE)
		return (error_token(PIPE));
	while (lex[i + 1] && !(is_a_token(lex[i][0]) && is_a_token(lex[i + 1][0])))
		i++;
	if (lex[i + 1])
		return (error_token(lex[i + 1][0]));
	return (0);
}

char	*create_token(t_token token)
{
	char	*str;
	str = malloc(sizeof(char) * 2);
	str[0] = token;
	str[1] = 0;
	return (str);
}

char	*ft_ttoa(t_token token)
{
	if (token == PIPE)
		return ("|");
	if (token == NEWLINE)
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
		return (create_token(NEWLINE));
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
