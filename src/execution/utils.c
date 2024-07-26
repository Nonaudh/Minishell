/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:22:18 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 15:22:34 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_getenv(char *str, char **env)
{
	int	i;

	i = 0;
	if (!str || !env)
		return (NULL);
	while (env[i]
		&& (ft_strncmp(str, env[i], ft_strlen(str))
			|| !(ft_strlen(str) == ft_strchr_index(env[i], '='))))
		i++;
	if (env[i])
		return (env[i] + ft_strlen(str) + 1);
	return (NULL);
}

void	error_is_a_directory(char *dir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

void	error_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
