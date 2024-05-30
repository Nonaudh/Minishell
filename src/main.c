#include "../inc/minishell.h"

	// GREAT,
	// LESS,
	// PIPE,

int	is_a_token(char c)
{
	if (c == '|' || c == '>' || c == '<')
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
	if (token == GREAT)\
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

	while (str[i])
	{
		if (is_a_token(str[i]) || (str[i] > 32 && (str[i + 1] <= 32 || is_a_token(str[i + 1]))))
			count++;
		i++;
		if (str[i] == 34)
		{
			i++;
			while (str[i] && str[i] != 34)
				i++;
			if (!str[i])
			{
				printf("Error quote\n");
				return (-1);
			}			
		}
		if (str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
			if (!str[i])
			{
				printf("Error quote\n");
				return (-1);
			}
		}
	}
	return (count);
}


char	*next_argv(char *line, int *x)
{
	int	i = *x;
	char	*str;
	int	start;
	int	end;

	while (line[i] <= 32)
		i++;
	if (is_a_token(line[i]))
	{
		str = ft_substr(line, i, 1);
		*x = i + 1;
		return (str);
	}
	start = i;
	while (line[i] > 32 && !is_a_token(line[i]))
	{
		if (line[i] == 34)
		{
			i++;
			while (line[i] != 34)
				i++;
		}
		if (line[i] == 39)
		{
			i++;
			while (line[i] != 39)
				i++;
		}
		i++;
	}
	str = ft_substr(line, start, i - start);
	*x = i;
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

	argc = count_argc(line);
	printf("argc; %d\n", argc);
	if (argc == -1)
		return (NULL);
	lex = fill_lex(line, argc);

	
	return (lex);
}

char	*unquote(char *str)
{
	int		i = 0;
	int		x = 0;
	char	*lex;

	lex = malloc(sizeof(char) *(ft_strlen(str) + 1));
	if (!lex)
		return (NULL);
	while (str[i])
	{
		if (str[i] == 34)
		{
			i++;
			while (str[i] != 34)
			{
				lex[x] = str[i];
				i++;
				x++;
			}
			i++;
		}
		if (str[i] == 39)
		{
			i++;
			while (str[i] != 39)
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

char	*add_env_value(char *str, int start, int *i, char **env)
{
	char	*env_value;
	int x = 0;
	char	*little;

	little = ft_substr(str, start, *i - start);
	printf("little %s\n", little);
	while (env[x] && !ft_strnstr(env[x], little, ft_strlen(little)))
		x++;
	if (env[x])
		return (env[x]);
	return (NULL);
}

char	*expand_env(char *str, char **env)
{
	int	i = 0;
	char	*lex;
	int		start;

	lex = malloc(sizeof(char) * 1);
	lex[0] = 0;
	while(str[i])
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		lex = ft_strjoin(lex, ft_substr(str, start, i - start));

		if (str[i] == '$')
		{
			i++;
			start = i;
			while (ft_isalpha(str[i]))
				i++;
			lex = ft_strjoin(lex, add_env_value(str, start, &i, env));
		}
		i++;
	}
	printf("str; %s\n", lex);
	return (str);
}

int	there_is_a_dollar(char *str)
{
	int	i = 0;

	while (str[i] && str[i] != '$')
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
		printf("%s\n", lex[i]);
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
	while (line)
	{
		printf("%s", line);
		minishell(line, env);
		printf("\n");
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	free_the_tab(env);
	return (0);
}
