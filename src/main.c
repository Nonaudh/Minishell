#include "../inc/minishell.h"

int	minishell(char *line, char **env, int exit_status)
{
	char **lex;
	t_commands *cmd;

	if (!line)
		return (1);
	lex = lexing(line, env, exit_status);
	if (!lex)
		return (2);
	print_tab(lex);
	cmd = parsing(lex, env);
	free_the_tab(lex);
	return (0);
}

char	*add_newline(char *line)
{
	char	*new_line;

	new_line = malloc(sizeof(char) * (ft_strlen(line) + 2));
	ft_memcpy(new_line, line, ft_strlen(line));
	new_line[ft_strlen(line)] = '\n';
	new_line[ft_strlen(line) + 1] = 0;
	free(line);
	return (new_line);
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line = (void *)1;
	char **env;
	int i = 0;
	int fd;
	int	exit_status = 1;

	fd = open("cmd", O_RDONLY);
	if (fd == -1)
		return (1);
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	if (argc == 1)
	{
			while (line && i < 10)
		{
			line = get_next_line(fd);
			printf("%s", line);
			exit_status = minishell(line, env, exit_status);
			printf("\n");
			free(line);
			i++;	
		}
	}
	else
	{
		while (i == 0 || ft_strncmp(line, "\n", 1))
		{	
			if (i)
				free(line);
			line = readline("Minishell: ");
			line = add_newline(line);
			exit_status = minishell(line, env, exit_status);
			printf("\n");
			i = 1;
		}	
	}
	free(line);
	free_the_tab(env);
	return (exit_status);
}
