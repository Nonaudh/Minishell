#include "../inc/minishell.h"

int	minishell(char *line, char **env)
{
	char **lex;

	if (!line)
		return (1);
	lex = lexing(line, env);
	if (!lex)
		return (1);
	print_tab(lex);
	free_the_tab(lex);		
	return (0);
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line = (void *)1;
	char **env;
	int i = 0;
	int fd;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	while (line && i < 10)
	{
		line = get_next_line(fd);
		printf("%s", line);
		minishell(line, env);
		printf("\n");
		free(line);
		i++;
	}
	free_the_tab(env);
	return (0);
}
