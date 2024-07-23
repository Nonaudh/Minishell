/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:57:13 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:57:15 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmp_env(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '=' && s1[i] == s2[i])
		i++;
	if (s1[i] == s2[i])
		return (0);
	if (s1[i] == '=')
		return (0);
	if (s2[i] == '=')
		return (1);
	if (s1[i] < s2[i])
		return (0);
	if (s1[i] > s2[i])
		return (1);
	return (0);
}

void	print_env_value(char *str)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	ft_putstr_fd("declare -x ", 1);
	while (str[i])
	{
		ft_putchar_fd(str[i], 1);
		if (str[i] == '=')
		{
			ft_putchar_fd('\"', 1);
			equal = 1;
		}
		i++;
	}
	if (equal)
		ft_putchar_fd('\"', 1);
	ft_putchar_fd('\n', 1);
}

void	print_tab_int(int *tab, int size, char **env)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (i < size)
	{
		print_env_value(env[tab[i]]);
		i++;
	}
}

int	print_sort_env(char **env, int size_env)
{
	int	*tab;
	int	i;
	int	j;
	int	rank;

	j = 0;
	tab = malloc(sizeof(int) * size_env);
	if (!tab)
		return (1);
	while (j < size_env)
	{
		i = 0;
		rank = 0;
		while (env[i])
		{
			if (cmp_env(env[j], env[i]))
				rank++;
			i++;
		}
		tab[rank] = j;
		j++;
	}
	print_tab_int(tab, size_env, env);
	free(tab);
	return (0);
}

char	**ft_export(t_cmd *cmd, int size, int *ex_st)
{
	int	size_export;

	size_export = count_arg(&cmd->arg[1]);
	*ex_st = 0;
	if (size_export == 0)
	{
		if (print_sort_env(cmd->env, count_arg(cmd->env)))
			return (NULL);
	}
	if (size_export > 0)
	{
		cmd->env = add_env_variable(cmd, size, ex_st);
	}
	return (cmd->env);
}
