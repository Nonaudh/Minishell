/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:36:30 by bdany             #+#    #+#             */
/*   Updated: 2024/06/18 15:18:19 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_pwd(t_commands *cmd, int *exit_status)
{
	char	cwd[1024];

	*exit_status = 0;
	if (check_option(cmd->arg))
	{	
		error_option(cmd, exit_status, 2);
		return (0);
	}
		
	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	return (1);
}
