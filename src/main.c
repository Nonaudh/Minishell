#include "../inc/minishell.h"

int	minishell(char *line, char **env, int exit_status)
{
	char **lex;
	int size;
	t_commands *cmd;

	if (!line)
		return (1);
	lex = lexing(line, env, exit_status);
	if (!lex)
		return (2);
	//print_tab(lex);
	size = count_cmd(lex);
	if (size == 0)
	{
		free_the_tab(lex);
		return (0);
	}
	cmd = parsing(lex, env, size, exit_status);
	exit_status = execution(cmd, size);
	free_the_tab(lex);
	free_struct_cmd(cmd, size);
	if (sig == 1)
		return (69);
	return (exit_status);
}

void	handle_sigstp(int sig)
{
	sigset_t	sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, sig);
	sig = 1;
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line = (void *)1;
	char **env;
	int i = 0;
	int fd;
	int	exit_status = 1;
	sig = 0;

	struct sigaction sa;
	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigstp;
	sigaction(SIGINT, &sa, NULL);
	//sa.sa
	

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
			if (line)
				line[ft_strlen(line) - 1] = 0;
			printf("%s", line);
			printf("\n");
			exit_status = minishell(line, env, exit_status);
			free(line);
			i++;	
		}
	}
	else
	{
		while (line && (i == 0 || ft_strncmp(line, "\0", 1)))
		{	
			if (i)
				free(line);
			line = readline("Minishell: ");
			if (sig == 1)
			{
					//rl_on_new_line ();
					ft_putendl_fd("SIGNAL", 2);
					sig = 0;
			}
			
			if (line && *line)
				add_history(line);
			exit_status = minishell(line, env, exit_status);
			i = 1;
		}
		rl_clear_history();
	}
	free(line);
	free_the_tab(env);
	return (exit_status);
}
