/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:58:42 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:58:45 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_limiter_hd(char *limiteur)
{
	ft_putstr_fd("bash: warning: here-document ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiteur, 2);
	ft_putstr_fd("')\n", 2);
}

void	error_ambiguous_redirect(int *ex_st)
{
	*ex_st = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("$FILE", 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

void	error_open_file(char *str, int *ex_st)
{
	*ex_st = 1;
	ft_putstr_fd("minishell: ", 2);
	perror(str);
}

t_cmd	*error_struct_cmd(t_cmd *cmd, int size, int *ex_st)
{
	*ex_st = 1;
	free_struct_cmd(cmd, size);
	return (NULL);
}
