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
		str = find_token(line[i]);
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
		if (print_token(lex[i][0]))
			;
		else
			printf("%s\n", lex[i]);
		i++;
	}
	lex[i] = NULL;
	return (lex);
}

char	**lexing(char *line)
{
	int		argc;
	char	**lex;

	argc = count_argc(line);
	printf("argc; %d\n", argc);
	if (argc == -1)
		return (NULL);
	lex = fill_lex(line, argc);

	return (NULL);
}

int	minishell(char *line)
{
	char **lex;

	lex = lexing(line);

	return (0);
}

int main(void)
{
	char *line;
	int i = 0;
	int fd;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		minishell(line);
		printf("\n");
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	return (0);
}
