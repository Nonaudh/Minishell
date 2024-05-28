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
				printf("Error quote\n");
		}
		if (str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
			if (!str[i])
				printf("Error quote\n");
		}
	}
	return (count);
}

char	*next_argv(char *line, int *x)
{
	while (line[*x])
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
	return (lex);
}

char	**lexing(char *line)
{
	int		argc;
	char	**lex;

	argc = count_argc(line);
	printf("argc; %d\n", argc);
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
