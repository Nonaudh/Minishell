/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:36:30 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 17:57:23 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_pwd(t_cmd *cmd, int *ex_st)
{
	char	cwd[1024];

	*ex_st = 0;
	if (check_option(cmd->arg))
	{
		error_option(cmd, ex_st, 2);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	return (1);
}
