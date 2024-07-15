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

int	error_arg_export(char *str, int *exit_status)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*exit_status = 1;
	return (1);
}

int check_argument_export(char *str, int *exit_status)
{
	int i;

	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (error_arg_export(str, exit_status));
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (error_arg_export(str, exit_status));
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (error_arg_export(str, exit_status));
	return (0);
}

int search_arg_in_env(char *str, char *env)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+' && env[i] && env[i] != '=')
	{
		if (str[i] != env[i])
			return (1);
		i++;
	}
	if ((!str[i] || str[i] == '=' || str[i] == '+') && (!env[i] || env[i] == '='))
		return (0);
	return (1);

}

char *replace(char *str, char *env)
{
	free(env);
	env = ft_strdup(str);
	return (env);
}

char	*join_arg(char *str, char *env)
{
	char *new_env;

	new_env = ft_strjoin(env, str);
	free(env);
	return (new_env);
}

char *replace_with_arg(char *str, char *env)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	if (!str[i])
		return (env);
	if (str[i] == '=')
		env = replace(str, env);
	if (str[i] == '+')
		env= join_arg(str + i + 2, env);
	return(env);
}

char	**realloc_and_copy_env(char **env_tmp, int size)
{
	char	**env;
	int		i;

	i = 0;
	env = malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (NULL);
	while (env_tmp[i])
	{
		env[i] = ft_strdup(env_tmp[i]);
		i++;
	}
	env[i] = NULL;
	free_the_tab(env_tmp);
	return (env);
}

char	*create_without_plus(char *str)
{
	char *env;
	int i;

	i = 0;
	env = malloc(sizeof(char) * ft_strlen(str));
	if (!env)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '+')
			str++;
		env[i] = str[i];
		i++;
	}
	env[i] = 0;
	return (env);
}

char **create_arg(char *str, char **env_tmp)
{
	int	i;
	char **env;
	int		size_env;

	size_env = count_arg(env_tmp);
	env = realloc_and_copy_env(env_tmp, size_env + 1);
	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	if (!str[i] || str[i] == '=')
		env[size_env] = ft_strdup(str);
	else
		env[size_env] = create_without_plus(str);
	env[size_env + 1] = NULL;
	return (env);
}

char	**add_arg_to_env(char *str, char **env, int *exit_status)
{
	int i;

	i = 0;
	if (check_argument_export(str, exit_status))
		return (env);
	while (env[i] && search_arg_in_env(str, env[i]))
		i++;
	if (env[i])
		env[i] = replace_with_arg(str, env[i]);
	else
		env = create_arg(str, env);
	return (env);
}

char	**add_env_variable(t_commands *cmd, int *exit_status)
{
	int		i;

	i = 1;
	while (cmd->arg[i])
	{
		cmd->env = add_arg_to_env(cmd->arg[i], cmd->env, exit_status);
		i++;
	}
	return (cmd->env);
}

int	print_sort_env(char **env)
{
	int	*tab;
	int	size;
	int	i;
	int	j;
	int	rank;

	j = 0;
	size = count_arg(env);
	tab = malloc(sizeof(int) * size);
	while (j < size)
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
	print_tab_int(tab, size, env);
	free(tab);
	return (0);
}

char	**ft_export(t_commands *cmd, int *exit_status)
{
	if (count_arg(&cmd->arg[1]) == 0)
	{
		print_sort_env(cmd->env);
	}
	if (count_arg(&cmd->arg[1]) > 0)
	{
		cmd->env = add_env_variable(cmd, exit_status);
	}
	return (cmd->env);
}