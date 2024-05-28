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
		}
		if (str[i] == 39)
		{
			i++;
			while (str[i] && str[i] != 39)
				i++;
		}
	}
	return (count);
}

int	minishell(char *line)
{
	int	argc;
	char **lex;

	argc = count_argc(line);
	printf("argc; %d\n", argc);


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
	if (line)
		line[ft_strlen(line) - 1] = 0;
	while (line)
	{
		printf("%s\n", line);
		minishell(line);
		printf("\n");
		free(line);
		line = get_next_line(fd);
		if (line)
			line[ft_strlen(line) - 1] = 0;
	}
	free(line);
	return (0);
}
