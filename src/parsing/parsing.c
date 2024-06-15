#include "../../inc/minishell.h"

int count_cmd(char **lex)
{
	int count;
	int i;

	count = 1;
	i = 0;
	while (lex[i])
	{
		if (lex[i][0] == PIPE)
			count++;
		i++;
	}
	return (count);
}

int	nb_of_arg(char **lex, int x)
{
	int	count = 0;

	while (lex[x][0] != PIPE || lex[x][0] != T_NEWLINE)
	{
		if (is_a_token(lex[x][0]))
			x+=2;
		else
		{
			x++;
			count++;			
		}
	}
	return (count);
}

t_commands *fill_the_struct(char **lex, char **env, int size)
{
	t_commands *cmd;
	int	i = 0;
	int	x = 0;

	cmd = malloc(sizeof(t_commands) * size);
	while (i < size)
	{
		cmd[i].arg = malloc(sizeof(nb_of_arg(lex, x)));
		i++;
	}
}

t_commands  *parsing(char **lex, char **env)
{
	t_commands *cmd;
	int size;

	size = count_cmd(lex);
	cmd = fill_the_struct(lex, env, size); 
	return (NULL);
}