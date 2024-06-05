#include "../../inc/minishell.h"

char	*unquote(char *str)
{
	int		i = 0;
	int		x = 0;
	int		quote;
	char	*lex;

	lex = malloc(sizeof(char) *(ft_strlen(str) + 1));
	if (!lex)
		return (NULL);
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			quote = str[i];
			i++;
			while (str[i] != quote)
			{
				lex[x] = str[i];
				i++;
				x++;
			}
			i++;
		}
		if (str[i] && str[i] != 34 && str[i] != 39)
		{
			lex[x] = str[i];
			i++;
			x++;
		}
	}
	lex[x] = 0;
	free(str);
	return (lex);
}

char	*add_env_value(char *str, int start, int i, char **env)
{
	char	*env_value;
	int x = 0;
	char	*little;
	int		little_len;

	little = ft_substr(str, start, i - start);
	little_len = ft_strlen(little);
	while (env[x] && (little_len  < ft_strchr_index(env[x], '=')
		|| !ft_strnstr(env[x], little, ft_strchr_index(env[x], '='))))
		x++;
	free(little);
	if (env[x])
		return (env[x] + little_len + 1);
	return (ft_strchr(str, 0));
}

char	*expand_env(char *str, char **env)
{
	int	i = 0;
	char	*lex = NULL;
	int		start;

	while(str[i])
	{
		start = i;
		while (str[i] && (str[i] != '$' || !ft_isalnum(str[i + 1])) && str[i] != '\'')
			i++;
		if (str[i] == '\'')
			i += (ft_strchr_index(&str[i + 1], '\'') + 2);
		lex = ft_strjoin_dup_frees(lex, ft_substr(str, start, i - start));
		if (str[i] == '$')
		{
			i++;
			start = i;
			while (ft_isalnum(str[i]))
				i++;
			lex = ft_strjoin_dup_free_s1(lex, add_env_value(str, start, i, env));
		}
	}
	free(str);
	return (lex);
}

char	**expand_lex(char **lex, char **env)
{
	int i = 0;

	while (lex[i])
	{
		if (env_variable_detected(lex[i]))
			lex[i] = expand_env(lex[i], env);
		lex[i] = unquote(lex[i]);
		i++;
	}
	return (lex);
}
