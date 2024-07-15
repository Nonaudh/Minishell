#include "../inc/minishell.h"

int	g_sig_flag = 0;

void	handle_sigstp_mini(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 1;
		ft_putstr_fd("\n", 1);
	}
}

void	set_signal_exec(void)
{
	struct sigaction sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigstp_mini;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}

void	print_cmd(t_commands *cmd, int size)
{
	int i = 0;

	while (i < size)
	{
		print_tab(cmd[i].arg);
		printf("int; %d\n", cmd[i].fd_in);
		printf("out; %d\n\n", cmd[i].fd_out);
		i++;
	}
}

char	**minishell(char *line, char **env, int *exit_status)
{
	char **lex;
	int size;
	t_commands *cmd;
	


	if (!line)
		return (env);
	lex = lexing(line, env, exit_status);
	if (!lex)
		return (env);
	size = count_cmd(lex);
	if (size == 0)
	{
		free_the_tab(lex);
		return (env);
	}
	cmd = parsing(lex, env, size, exit_status);
	free_the_tab(lex);
	if (!cmd)
	{
		*exit_status = 130;
		return (env);
	}
	// print_cmd(cmd, size);
	// return (0);
	set_signal_exec();
	env = execution(cmd, size, exit_status);
	//printf("exit; %d\n", *exit_status);
	free_struct_cmd(cmd, size);
	return (env);
}

void	handle_sigstp(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 1;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sig_main(void)
{
	struct sigaction sa;
	
	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigstp;
	sigaction(SIGINT, &sa, NULL);
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line = (void *)1;
	char **env;
	int i = 0;
	int	exit_status = 1;

	
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	while (line)
	{
		handle_sig_main();
		if (line != (void *)1)
			free(line);
		line = readline("Minishell: ");
		if (g_sig_flag)
		{
			exit_status = 42;
			g_sig_flag = 0;
		}
		if (line && *line)
			add_history(line);
		env = minishell(line, env, &exit_status);
	}
	rl_clear_history();
	free_the_tab(env);
	return (exit_status);
}
