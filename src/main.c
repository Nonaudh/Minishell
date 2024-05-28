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

int	lexing(char *str)
{
	int i = 0;
	int count = 0;

	while (str[i])
	{
		if (is_a_token(str[i]) || (str[i] != 32 && (str[i + 1] == 32 || !str[i + 1] || is_a_token(str[i + 1]))))
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

int main(void)
{
	char *line;
	int i = 0;
	int fd;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);

	i = lexing(line);
	printf("i; %d\n", i);

	free(line);
	return (0);
}
