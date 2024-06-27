#include "../../inc/minishell.h"

char	*ft_getenv(char *str, char **env)
{
	int i;

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