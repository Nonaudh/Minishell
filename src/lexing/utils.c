/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdany <bdany@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:09:12 by bdany             #+#    #+#             */
/*   Updated: 2024/07/23 14:29:10 by bdany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_struct_cmd(t_cmd *cmd, int size)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (i < size)
	{
		if (cmd[i].arg)
			free_the_tab(cmd[i].arg);
		i++;
	}
	free(cmd);
}

char	**ft_str_tab_dup(char **tab1)
{
	int		i;
	char	**tab2;

	i = 0;
	if (!tab1)
		return (NULL);
	while (tab1[i])
		i++;
	tab2 = malloc(sizeof(char *) * (i + 1));
	if (!tab2)
		return (NULL);
	i = 0;
	while (tab1[i])
	{
		tab2[i] = ft_strdup(tab1[i]);
		if (!tab2[i])
			return (NULL);
		i++;
	}
	tab2[i] = NULL;
	return (tab2);
}

char	*ft_strjoin_dup_free_s1(char const *s1, char const *s2)
{
	char	*str;

	if (!s1 && s2)
		str = ft_strdup(s2);
	else
	{
		str = ft_strjoin(s1, s2);
		free((char *)s1);
	}
	return (str);
}

char	*ft_strjoin_dup_frees(char const *s1, char const *s2)
{
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	if (s1 && !s2)
	{
		str = ft_strdup(s1);
		free ((char *)s1);
	}
	if (!s1 && s2)
	{
		str = ft_strdup(s2);
		free((char *)s2);
	}
	else
	{
		str = ft_strjoin(s1, s2);
		free((char *)s2);
		free((char *)s1);
	}
	return (str);
}

int	ft_strchr_index(char const *str, int c)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != (char)c)
		i++;
	if (str[i] == (char)c)
		return (i);
	return (-1);
}
