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
		cmd->fd_in = STDIN_FILENO;
		cmd->fd_out = STDOUT_FILENO;
		i++;
	}
	return (0);
}

int	less_infile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open(lex[*x], O_RDONLY);
	if (cmd->fd_in == -1)
		perror(lex[*x]);
	(*x)++;
	return (0);
}

int	check_quote(char *limiter)
{
	int	i;

	i = 0;
	while (limiter[i] && limiter[i] != 34 && limiter[i] != 39)
		i++;
	if (limiter[i])
		return (1);
	return (0);
}

int	write_here_doc(char *limiter, char **env)
{
	int		fd_hd;
	char	*line;
	int		quote;
	char	*limiter_copy;

	quote = check_quote(limiter);
	limiter_copy = unquote(ft_strdup(limiter));
	fd_hd = open("/tmp/here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_hd == -1)
		perror("here_doc");
	line = readline(">");
	while (line && (ft_strlen(line) != ft_strlen(limiter_copy) 
			|| !ft_strnstr(line, limiter_copy, ft_strlen(limiter_copy))))
	{
		if (quote && env_variable_detected(line))
				line = expand_env(line, env, 0);
		ft_putendl_fd(line, fd_hd);
		free(line);
		line = readline(">");
	}
	if (!line)
		printf("bash: warning: here-document delimited by end-of-file (wanted `%s')", limiter_copy);
	free(line);
	free(limiter_copy);
	close(fd_hd);
	return (0);
}

int	here_doc_infile(char **lex, t_commands *cmd, int *x, char **env)
{
	(*x)++;
	write_here_doc(lex[*x], env);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open("/tmp/here_doc", O_RDONLY);
	if (cmd->fd_in == -1)
		perror("here_doc_infile");
	if (unlink("/tmp/here_doc"))
		printf("Error unlink infile\n");
	(*x)++;
	return (0);
}

int	great_outfile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[*x], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
		perror(lex[*x]);
	(*x)++;
	return (0);
}

int	great_great_outfile(char **lex, t_commands *cmd, int *x)
{
	(*x)++;
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[*x], O_CREAT | O_APPEND | O_WRONLY, 0644);
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

int	put_in_the_struct(char **lex, t_commands *cmd, int *x, char **env)
{
	if (lex[*x][0] == LESS)
		less_infile(lex, cmd, x);
	else if (lex[*x][0] == LESSLESS)
		here_doc_infile(lex, cmd, x, env);
	else if (lex[*x][0] == GREAT)
		great_outfile(lex, cmd, x);
	else if (lex[*x][0] == GREATGREAT)
		great_great_outfile(lex, cmd, x);
	else
		is_an_argument(lex, cmd, x);
	return (0);
}

int	piped(t_commands *cmd_in, t_commands *cmd_out)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd))
		printf("Error pipe\n");
	if (cmd_in->fd_out == STDOUT_FILENO)
		cmd_in->fd_out = pipe_fd[1];
	else
		close(pipe_fd[1]);
	if (cmd_out->fd_in == STDIN_FILENO)
		cmd_out->fd_in = pipe_fd[0];
	else
		close(pipe_fd[0]);
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
		while (lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		{
			put_in_the_struct(lex, &cmd[i], &x, env);
		}
		if (lex[x][0] == PIPE)
			piped(&cmd[i], &cmd[i + 1]);
		i++;
	}
	return (cmd);
}

t_commands  *parsing(char **lex, char **env)
{
	t_commands *cmd;
	int size;

	size = count_cmd(lex);
	cmd = fill_the_struct(lex, env, size); 
	return (cmd);
}
