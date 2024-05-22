#include "../inc/minishell.h"

	// LEX,
	// GREAT,
	// LESS,
	// PIPE,

char	find_character(char c)
{
	if (c == '>')
		return (GREAT);
	if (c == '<')
		return (LESS);
	if (c == '|')
		return (PIPE);
	return (c);
}

void	print_buffer(char *str)
{
	int	i = 0;

	while (str[i])
	{		
		if (str[i] == GREAT)
			printf("GREAT");
		else if (str[i] == LESS)
			printf("LESS");
		else if (str[i] == PIPE)
			printf("PIPE");
		else
			printf("%c", str[i]);
		i++;
	}
}

void	simple_quote_lexing(char *line, char *str, int *i, int *x)
{
	while (line[*i] && line[*i] != 39)
	{
		str[*x] = line[*i];
		(*i)++;
		(*x)++;
	}
	if (!line[*i])
	{
		printf("Error simple quote\n");
		return ;
	}
}

void	lexing(char *line, char *str)
{
	int i = 0;
	int	x = 0;

	while (line[i])
	{
		if (line[i] == 39)
		{
			i++;
			simple_quote_lexing(line, str, &i, &x);
			i++;
		}
		str[x] = find_character(line[i]);
		i++;
		x++;
	}
	str[x] = 0;
}

int main(void)
{
	char str[BIG_BUFFER];
	char *line;
	int i = 0;
	int fd;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);

	lexing(line, str);

	free(line);
	print_buffer(str);
	return (0);
}
