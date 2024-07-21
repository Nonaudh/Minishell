#include "../../inc/minishell.h"

int	not_numerical_value(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] && (ft_isdigit(arg[i]) || arg[i] == '+' || arg[i] == '-'))
		i++;
	if (i == 0)
		return (1);
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	if (arg[i])
		return (1);
	return (0);
}

int	positive_exit(int nb)
{
	while (nb > 255)
		nb -= 256;
	return (nb);
}

int	negative_exit(int nb)
{
	while (nb < 0)
		nb += 256;
	return (nb);
}

int	exit_value(char *arg)
{
	int	nb;

	nb = ft_atol(arg);
	if (nb > 0)
		return (positive_exit(nb));
	return (negative_exit(nb));
}

int	return_value_exit(char *arg, int *exit_status)
{
	if (!arg)
	{
		return (0);
	}	
	else if (not_numerical_value(arg))
	{
		*exit_status = 2;
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (1);
	}	
	else
		*exit_status = exit_value(arg);
	return (0);
}

int	check_arg_exit(char **arg, int *exit_status)
{
	int	i;

	i = 0;
	if (return_value_exit(arg[0], exit_status))
		return (1);
	if (arg[0] && arg[1])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
        *exit_status = 1;
		return (0);
	}	
	return (1);
}

char	**ft_exit(t_commands *cmd, int *exit_status)
{
	if (check_arg_exit(cmd->arg + 1, exit_status))
		return (NULL);
	return (cmd->env);
}