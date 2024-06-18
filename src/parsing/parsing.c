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

int	set_struct_to_default(t_commands *cmd, char **env, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		cmd->env = env;
		cmd->fd_in = 0;
		cmd->fd_out = 1;
		i++;
	}
	return (0);
}

int	less_infile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_in != 0)
		close(cmd->fd_in);
	cmd->fd_in = open(lex[*x], O_RDONLY, O_CREAT);
	if (cmd->fd_in == -1)
		perror(lex[*x]);
	(*x)++;
	return (0);
}

int	great_outfile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[*x], O_WRONLY, O_CREAT);
	if (cmd->fd_out == -1)
		perror(lex[*x]);
	(*x)++;
	return (0);
}

int	great_great_outfile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[*x], O_WRONLY, O_CREAT, O_APPEND);
	if (cmd->fd_out == -1)
		perror(lex[*x]);
	(*x)++;
	return (0);
}

int	is_an_argument(char **lex, t_commands *cmd, int *x)
{
	int	i;

	i = 0;
	while (cmd->arg[i])
		i++;
	cmd->arg[i] = ft_strdup(lex[*x]);
	if (!cmd->arg[i])
		printf("Error malloc\n");
	cmd->arg[i + 1] = NULL;
	(*x)++;
	return (0);
}

int	put_in_the_struct(char **lex, t_commands *cmd, int *x)
{
	if (lex[*x][0] == LESS)
		less_infile(lex, cmd, x);
	else if (lex[*x][0] == GREAT)
		great_outfile(lex, cmd, x);
	else if (lex[*x][0] == GREATGREAT)
		great_great_outfile(lex, cmd, x);
	else
		is_an_argument(lex, cmd, x);
	return (0);
}


t_commands *fill_the_struct(char **lex, char **env, int size)
{
	t_commands *cmd;
	int	i = 0;
	int	x = 0;

	cmd = malloc(sizeof(t_commands) * size);
	set_struct_to_default(cmd, env, size);
	while (i < size)
	{
		cmd[i].arg = malloc(sizeof(nb_of_arg(lex, x)));
		cmd[i].arg[0] = NULL;
		while (lex[x][0] != PIPE || lex[x][0] != T_NEWLINE)
		{
			put_in_the_struct(lex, &cmd[i], &x);
		}
		i++;
	}
	return (NULL);
}

t_commands  *parsing(char **lex, char **env)
{
	t_commands *cmd;
	int size;

	size = count_cmd(lex);
	cmd = fill_the_struct(lex, env, size); 
	return (NULL);
}