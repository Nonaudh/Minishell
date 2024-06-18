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

char	*add_env_value(char *str, int size, char **env, int exit_status)
{
	char	*env_value;
	int x = 0;
	char	*little;
	int		little_len;

	little = ft_substr(str, 0, size);
	little_len = ft_strlen(little);
	if (little_len == 1 && little[0] == '?')
	{
		free(little);
		return(ft_itoa(exit_status));
	}
	while (env[x] && (little_len  < ft_strchr_index(env[x], '=')
		|| !ft_strnstr(env[x], little, ft_strchr_index(env[x], '='))))
		x++;
	free(little);
	if (env[x])
		return (ft_substr(env[x], little_len + 1, ft_strlen(env[x]) - (little_len + 1))); //env[x] + little_len + 1 );
	return (ft_strdup("\0"));
}

char	*expand_env(char *str, char **env, int exit_status)
{
	int	i = 0;
	char	*lex = NULL;
	int		start;

	while(str[i])
	{
		start = i;
		while (str[i] && (str[i] != '$' || !ft_isprint(str[i + 1])) && str[i] != '\'')
			i++;
		if (str[i] == '\'')
			i += (ft_strchr_index(&str[i + 1], '\'') + 2);
		lex = ft_strjoin_dup_frees(lex, ft_substr(str, start, i - start));
		if (str[i] == '$')
		{
			i++;
			start = i;
			while (ft_isprint(str[i]) && str[i] != '\'' && str[i] != '\"' && str[i - 1] != '?')
				i++;
			lex = ft_strjoin_dup_frees(lex, add_env_value(str + start, i - start, env, exit_status));
		}
	}
	free(str);
	return (lex);
}

char	**expand_lex(char **lex, char **env, int exit_status)
{
	int i = 0;

	while (lex[i])
	{
		if (i == 0 || lex[i - 1][0] != LESSLESS)
		{
			if (env_variable_detected(lex[i]))
				lex[i] = expand_env(lex[i], env, exit_status);
			lex[i] = unquote(lex[i]);			
		}
		i++;
	}
	return (lex);
}
