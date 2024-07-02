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

void	print_cmd(t_commands *cmd)
{
	int i = 0;
	
	while (cmd)
	{
		cmd++;
		i++;
		print_tab(cmd->arg);
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
		return (exit_status);
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
	if (g_sig_flag == 1)
	{
		g_sig_flag = 0;
		exit_status = 69;
	}
	return (exit_status);
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
		exit_status = minishell(line, env, exit_status);
	}
	rl_clear_history();
	free_the_tab(env);
	return (exit_status);
}
