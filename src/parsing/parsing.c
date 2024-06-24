#include "../../inc/minishell.h"

int count_cmd(char **lex)
{
	int count;
	int i;

	count = 1;
	i = 0;
	if (lex[0][0] == T_NEWLINE)
		return (0);
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
	int i = 0;

	while (lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
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
		cmd[i].env = env;
		cmd[i].fd_in = STDIN_FILENO;
		cmd[i].fd_out = STDOUT_FILENO;
		i++;
	}
	return (0);
}

int	until_pipe_or_newline(char **lex)
{
	int	x;

	x = 0;
	while (lex[x] && lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		x++;
	return (x);
}

int	less_infile(char **lex, t_commands *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open(lex[1], O_RDONLY);
	if (cmd->fd_in == -1)
	{
		perror(lex[1]);
		return(until_pipe_or_newline(&lex[1]));
	}
	return (2);
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

int	write_here_doc(char **lex, char **env, int exit_status)
{
	int		fd_hd;
	char	*line;
	int		quote;

	quote = check_quote(lex[0]);
	lex[0] = unquote(lex[0]);
	fd_hd = open("/tmp/here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_hd == -1)
		perror("here_doc");
	line = readline(">");
	while (line && (ft_strlen(line) != ft_strlen(lex[0]) 
			|| !ft_strnstr(line, lex[0], ft_strlen(lex[0]))))
	{
		if (quote && env_variable_detected(line))
				line = expand_env(line, env, exit_status);
		ft_putendl_fd(line, fd_hd);
		free(line);
		line = readline(">");
	}
	if (!line)
		printf("bash: warning: here-document delimited by end-of-file (wanted `%s')", lex[0]);
	free(line);
	close(fd_hd);
	return (0);
}

int	here_doc_infile(char **lex, t_commands *cmd, char **env, int exit_status)
{
	write_here_doc(&lex[1], env, exit_status);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open("/tmp/here_doc", O_RDONLY);
	if (cmd->fd_in == -1)
		perror("here_doc_infile");
	else if (unlink("/tmp/here_doc"))
		printf("Error unlink infile\n");
	return (0);
}

int	great_outfile(char **lex, t_commands *cmd)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
		perror(lex[1]);
	return (0);
}

int	great_great_outfile(char **lex, t_commands *cmd)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[1], O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (cmd->fd_out == -1)
		perror(lex[1]);
	return (0);
}

int	is_an_argument(char **lex, t_commands *cmd)
{
	int	i;

	i = 0;
	while (cmd->arg[i])
		i++;
	cmd->arg[i] = ft_strdup(lex[0]);
	if (!cmd->arg[i])
		printf("Error malloc\n");
	cmd->arg[i + 1] = NULL;
	return (0);
}

int	put_in_the_struct(char **lex, t_commands *cmd, char **env, int exit_status)
{
	if (lex[0][0] == LESS)
		return (less_infile(lex, cmd));
	else if (lex[0][0] == LESSLESS)
		here_doc_infile(lex, cmd, env, exit_status);
	else if (lex[0][0] == GREAT)
		great_outfile(lex, cmd);
	else if (lex[0][0] == GREATGREAT)
		great_great_outfile(lex, cmd);
	else
	{
		is_an_argument(lex, cmd);
		return (1);
	}
	return (2);
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
	return (1);
}

t_commands *fill_the_struct(char **lex, char **env, int size, int exit_status)
{
	t_commands *cmd;
	int	i = 0;
	int	x = 0;

	cmd = malloc(sizeof(t_commands) * size);
	set_struct_to_default(cmd, env, size);
	while (lex[x][0] != T_NEWLINE)
	{
		cmd[i].arg = malloc(sizeof(nb_of_arg(lex, x) + 1));
		cmd[i].arg[0] = NULL;
		while (lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		{
			x += put_in_the_struct(&lex[x], &cmd[i], env, exit_status);
		}
		if (lex[x][0] == PIPE)
			x += piped(&cmd[i], &cmd[i + 1]);
		i++;
	}
	return (cmd);
}

t_commands  *parsing(char **lex, char **env, int size, int exit_status)
{
	t_commands *cmd;
	char **lex_tmp;

	lex_tmp = lex;
	cmd = fill_the_struct(lex, env, size, exit_status); 
	return (cmd);
}
