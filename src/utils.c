#include "../inc/minishell.h"

void	print_tab(char **tab)
{
	int i = 0;

	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

void	free_the_tab(char **tab)
{
	int i = 0;

	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_str_tab_dup(char **tab1)
{
	int	i = 0;
	char	**tab2;

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
