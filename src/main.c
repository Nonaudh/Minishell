#include "../inc/minishell.h"

int	g_sig_flag = 0;

void	handle_sig_mini(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 1;
		ft_putstr_fd("\n", 1);
	}
	if (signal == SIGQUIT)
	{
		g_sig_flag = 1;
		ft_putstr_fd("Quit (core dumped)\n", 1);
	}
}

void	set_signal_exec(void)
{
	struct sigaction sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &handle_sig_mini;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
	char	**new_env;
	t_commands *cmd;
	
	if (!line)
		return (env);
	lex = lexing(line, env, exit_status);
	size = count_cmd(lex);
	cmd = parsing(lex, env, size, exit_status);
	free_the_tab(lex);
	set_signal_exec();
	new_env = execution(cmd, size, exit_status);
	free_struct_cmd(cmd, size);
	if (new_env)
		return (new_env); 
	return (NULL);
}

void	handle_sig_main(int signal)
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

void	set_signal_main(void)
{
	struct sigaction sa;
	struct sigaction sa2;
	
	ft_bzero(&sa, sizeof(struct sigaction));
	ft_bzero(&sa2, sizeof(struct sigaction));
	sa.sa_handler = &handle_sig_main;
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa2, NULL);
}

int main(int argc, char **argv, char **env_tmp)
{
	char *line = (void *)1;
	char **env;
	int i = 0;
	int	exit_status = 0;

	
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	while (line)
	{
		set_signal_main();
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
		env_tmp = minishell(line, env, &exit_status);
		if (env_tmp)
			env = env_tmp;
		else
			line = NULL;
	}
	rl_clear_history();
	free_the_tab(env);
	return (exit_status);
}
