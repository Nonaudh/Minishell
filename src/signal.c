/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:05:49 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/24 10:05:51 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sig_mini(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 130;
		ft_putstr_fd("\n", 1);
	}
	if (signal == SIGQUIT)
	{
		g_sig_flag = 131;
		ft_putstr_fd("Quit (core dumped)\n", 1);
	}
}

void	set_signal_exec(void)
{
	struct sigaction	sa;
	struct sigaction	sa2;

	ft_bzero(&sa, sizeof(struct sigaction));
	ft_bzero(&sa2, sizeof(struct sigaction));
	sa.sa_handler = &handle_sig_mini;
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	handle_sig_main(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 130;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_main(void)
{
	struct sigaction	sa;
	struct sigaction	sa2;

	ft_bzero(&sa, sizeof(struct sigaction));
	ft_bzero(&sa2, sizeof(struct sigaction));
	sa.sa_handler = &handle_sig_main;
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa2, NULL);
}
