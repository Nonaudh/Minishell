#include "../../inc/minishell.h"

char	*ft_getenv(char *str, char **env)
{
	int i;

	i = 0;
	while (env[i] 
		&& !(ft_strlen(str) == ft_strchr_index(env[i], '=')
		&& ft_strnstr(env[i], str, ft_strlen(str))))
		i++;
	if (env[i])
		return (env[i] + ft_strlen(str) + 1);
	return (NULL);
}