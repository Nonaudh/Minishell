#include "../inc/minishell.h"

volatile int	sig = 0;

void	handle_sigstp_mini(int signal)
{
	if (signal == SIGINT)
	{
		sig = 1;
		// ft_putstr_fd("\n", 1);
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		return ;
	}
}

int	minishell(char *line, char **env, int exit_status)
{
	char **lex;
	int size;
	t_commands *cmd;
	
	struct sigaction sa;
	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigstp_mini;
	sigaction(SIGINT, &sa, NULL);

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
	{
		sig = 0;
		exit_status = 69;
	}
	return (exit_status);
}

void	handle_sigstp(int signal)
{
	if (signal == SIGINT)
	{
		sig = 1;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
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
		while (line)
		{	
			sigaction(SIGINT, &sa, NULL);
			if (i)
				free(line);
			line = readline("Minishell: ");
			if (sig)
			{
				exit_status = 42;
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
