#include "../inc/minishell.h"

	// GREAT,
	// LESS,
	// PIPE,

int	is_a_token(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '\n' || c == ' ')
		return (1);
	return (0);
}

char	*create_token(t_token token)
{
	char	*str;
	str = malloc(sizeof(str) * 2);
	str[0] = token;
	str[1] = 0;
	return (str);
}
int	print_token(t_token token)
{
	if (token == PIPE)
	{
		printf("PIPE\n");
		return (1);
	}
	if (token == GREAT)
	{
		printf("GREAT\n");
		return (1);
	}
	if (token == LESS)
	{
		printf("LESS\n");
		return (1);
	}
	return (0);	
}

char	*find_token(char c)
{
	if (c == '|')
		return (create_token(PIPE));
	if (c == '>')
		return (create_token(GREAT));
	if (c == '<')
		return (create_token(LESS));
	return (0);
}

int	count_argc(char *str)
{
	int i = 0;
	int count = 0;
	int	quote;

	while (str[i])
	{
		if (is_a_token(str[i]) || (str[i] > 32 && (str[i + 1] <= 32 || is_a_token(str[i + 1]))))
			count++;
		//i++;
		if (str[i] == 34 || str[i] == 39)
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				printf("Error quote\n");
				return (-1);
			}
		}
		i++;
	}
	return (count);
}


char	*next_argv(char *line, int *x)
{
	char	*str;
	int	start;

	if (!line[*x])
		return (NULL);	
	while (line[*x] && line[*x] != '\n' && line[*x] <= 32)
		(*x)++;
	if (is_a_token(line[*x]))
	{
		str = ft_substr(line, *x, 1);
		(*x)++;
		return (str);
	}
	start = *x;
	while (line[*x] > 32 && !is_a_token(line[*x]))
	{
		if (line[*x] == 34)
			*x += ft_strchr_index(&line[*x + 1], 34)+ 1;
		if (line[*x] == 39)
			*x += ft_strchr_index(&line[*x + 1], 39) + 1;
		(*x)++;
	}
	str = ft_substr(line, start, *x - start);
	return (str);
}

char	**fill_lex(char *line, int argc)
{
	char	**lex;
	int		i = 0;
	int		x = 0;

	lex = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		lex[i] = next_argv(line, &x);
		i++;
	}
	lex[i] = NULL;
	return (lex);
}

char	**lexing(char *line, char **env)
{
	int		argc;
	char	**lex;

	argc = count_argc(line) + 1;
	printf("argc; %d\n", argc);
	if (argc == 0)
		return (NULL);
	lex = fill_lex(line, argc);

	return (lex);
}

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
	return (ft_strchr(env[0], 0));
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
			while (ft_isalpha(str[i]))
				i++;
			lex = ft_strjoin_dup_free_s1(lex, add_env_value(str, start, i, env));
		}
	}
	free(str);
	return (lex);
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

char	**expand_lex(char **lex, char **env)
{
	int i = 0;

	while (lex[i])
	{
		if (there_is_a_dollar(lex[i]))
			lex[i] = expand_env(lex[i], env);
		lex[i] = unquote(lex[i]);
		i++;
	}
	return (lex);
}

int	minishell(char *line, char **env)
{
	char **lex;

	if (!line)
		return (1);
	lex = lexing(line, env);
	if (!lex)
		return (1);
	lex = expand_lex(lex, env);
	if (lex)
	{
		print_tab(lex);
		free_the_tab(lex);		
	}
	return (0);
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line;
	char **env;
	int i = 0;
	int fd;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	line = get_next_line(fd);
	while (line && i < 10)
	{
		printf("%s", line);
		minishell(line, env);
		printf("\n");
		free(line);
		line = get_next_line(fd);
		i++;
	}
	free(line);
	free_the_tab(env);
	return (0);
}
