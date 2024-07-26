/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:55:10 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:55:12 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_sig_flag = 0;

char	**minishell(char *line, char **env, int *exit_status)
{
	char	**lex;
	int		size;
	t_cmd	*cmd;

	if (!line)
	{
		free_the_tab(env);
		return (NULL);
	}
	lex = lexing(line, env, exit_status);
	size = count_cmd(lex);
	if (!size)
	{
		if (lex)
			free_the_tab(lex);
		return (env);
	}
	cmd = parsing(lex, env, size, exit_status);
	free_the_tab(lex);
	if (!cmd)
		return (env);
	set_signal_exec();
	env = execution(cmd, size, exit_status);
	free_struct_cmd(cmd, size);
	return (env);
}

int	check_signal(int exit_status)
{
	if (g_sig_flag)
	{
		exit_status = g_sig_flag;
		g_sig_flag = 0;
	}
	return (exit_status);
}

int	main(int argc, char **argv, char **env_tmp)
{
	char	*line;
	char	**env;
	int		exit_status;

	line = (void *)1;
	exit_status = 0;
	(void)argv;
	env = ft_str_tab_dup(env_tmp);
	if (!env)
		return (1);
	while (line && argc == 1)
	{
		set_signal_main();
		line = readline("minishell: ");
		exit_status = check_signal(exit_status);
		if (line && *line)
			add_history(line);
		env = minishell(line, env, &exit_status);
		free(line);
		if (!env)
			line = NULL;
	}
	rl_clear_history();
	free_the_tab(env);
	return (exit_status);
}
