/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:59:04 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:06 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	signal_here_doc(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 130;
		ft_putstr_fd("\n", 1);

		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		close (0);
	}
}

void	set_signal_here_doc(void)
{
	struct sigaction	sa;
	struct sigaction	sa2;

	ft_bzero(&sa, sizeof(struct sigaction));
	ft_bzero(&sa2, sizeof(struct sigaction));
	sa.sa_handler = &signal_here_doc;
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa2, NULL);
}
