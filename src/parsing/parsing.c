#include "../../inc/minishell.h"

int count_cmd(char **lex)
{
	int count;
	int i;

	count = 1;
	i = 0;
	if (!lex)
		return (0);
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
	char **env_tmp;

	i = 0;
	while (i < size)
	{
		cmd[i].arg = NULL;
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

int	less_infile(char **lex, t_commands *cmd, int *exit_status)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open(lex[1], O_RDONLY);
	if (cmd->fd_in == -1)
	{
		*exit_status = 1;
		perror(lex[1]);
		return(until_pipe_or_newline(lex));
	}
	if (lex[0][0] == LESSLESS)
	{
		if (unlink(lex[1]))
			perror("unlink : here_doc");
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

void	error_limiter_hd(char *limiteur)
{
	ft_putstr_fd("bash: warning: here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiteur, 2);
	ft_putstr_fd("')\n", 2);
}

int	write_here_doc(char **lex, char **env, int *exit_status, char *hd_name)
{
	int		fd_hd;
	char	*line;
	int		quote;

	quote = check_quote(lex[0]);
	lex[0] = unquote(lex[0]);
	fd_hd = open(hd_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
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
	if (!line && !g_sig_flag)
		error_limiter_hd(lex[0]);
	free(line);
	close(fd_hd);
	return (0);
}

char	*here_doc_name(void)
{
	if (access("/tmp/9036fddhsg", F_OK))
		return ("/tmp/9036fddhsg");
	if (access("/tmp/ie856fdgskd", F_OK))
		return ("/tmp/ie856fdgskd");
	if (access("/tmp/9ioqrac34g", F_OK))
		return ("/tmp/9ioqrac34g");
	if (access("/tmp/hfdyr649834hdf", F_OK))
		return ("/tmp/hfdyr649834hdf");
	return ("/tmp/jdhw782gtfsdh");
}

char	*here_doc_infile(char **lex, t_commands *cmd, char **env, int *exit_status)
{
	int	fdin_tmp;
	char	*hd_name;

	fdin_tmp = dup(STDIN_FILENO);
	set_signal_here_doc();
	hd_name = here_doc_name();
	write_here_doc(&lex[1], env, exit_status, hd_name);
	if (g_sig_flag)
	{
		//g_sig_flag = 0;
		dup2(fdin_tmp, STDIN_FILENO);
		if (unlink(hd_name))
			perror("unlike");
		close(fdin_tmp);
		return (lex[1]);
	}
	close(fdin_tmp);
	// if (cmd->fd_in != STDIN_FILENO)
	// 	close(cmd->fd_in);
	// cmd->fd_in = open("/tmp/here_doc", O_RDONLY);
	// if (cmd->fd_in == -1)
	// 	perror("here_doc_infile");
	// else if (unlink("/tmp/here_doc"))
	// 	perror("unlike");
	free(lex[1]);
	return (ft_strdup(hd_name));
}

int	great_outfile(char **lex, t_commands *cmd, int *exit_status)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
	{
		*exit_status = 1;
		perror(lex[1]);
		return(until_pipe_or_newline(lex));
	}
	return (2);
}

int	great_great_outfile(char **lex, t_commands *cmd, int *exit_status)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open(lex[1], O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (cmd->fd_out == -1)
	{
		*exit_status = 1;
		perror(lex[1]);
		return(until_pipe_or_newline(lex));
	}
	return (2);
}

int	is_an_argument(char **lex, t_commands *cmd)
{
	int	i;

	i = 0;
	if (!cmd->arg)
	{
		cmd->arg = malloc(sizeof(char *) * (nb_of_arg(lex, 0) + 1));
		if (!cmd->arg)
			return (-1);
		cmd->arg[i] = NULL;
	}
	if (!lex[0][0])
		return (1);
	while (cmd->arg[i])
		i++;
	cmd->arg[i] = ft_strdup(lex[0]);
	if (!cmd->arg[i])
		return (-1);
	cmd->arg[i + 1] = NULL;
	return (1);
}

int	put_in_the_struct(char **lex, t_commands *cmd, int *exit_status)
{
	if (lex[0][0] == LESS || lex[0][0] == LESSLESS)
		return (less_infile(lex, cmd, exit_status));
	// if (lex[0][0] == LESSLESS)
	// 	return (2);
	if (lex[0][0] == GREAT)
		return (great_outfile(lex, cmd, exit_status));
	if (lex[0][0] == GREATGREAT)
		return (great_great_outfile(lex, cmd, exit_status));
	return (is_an_argument(lex, cmd));
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

t_commands *error_struct_cmd(t_commands *cmd, int size, int *exit_status)
{
	*exit_status = 1;
	free_struct_cmd(cmd, size);
	return (NULL);
}

t_commands *fill_the_struct(char **lex, t_commands *cmd, int size, int *exit_status)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	x = 0;
	y = 0;
	while (y != -1 && lex[x][0] != T_NEWLINE)
	{
		while (y != -1 && lex[x][0] != PIPE && lex[x][0] != T_NEWLINE)
		{
			y = put_in_the_struct(&lex[x], &cmd[i], exit_status);
			x += y;
		}	
		if (y != -1 && lex[x][0] == PIPE)
		{
			y = piped(&cmd[i], &cmd[i + 1]);
			x += y;
		}		
		i++;
	}
	if (y == -1)
		return (error_struct_cmd(cmd, size, exit_status));
	return (cmd);
}

t_commands *create_struct_cmd(char **lex, char **env, int size, int *exit_status)
{
	t_commands *cmd;

	cmd = malloc(sizeof(t_commands) * (size + 1));
	if (!cmd)
		return (NULL);
	set_struct_to_default(cmd, env, size);
	if (!open_here_doc(lex, cmd, env, exit_status))
		cmd = fill_the_struct(lex, cmd, size, exit_status);
	else
	{
		free_struct_cmd(cmd, size);
		cmd = NULL;
	}
	return (cmd);
}

void	signal_here_doc(int signal)
{
	if (signal == SIGINT)
	{
		g_sig_flag = 130;
		ft_putstr_fd("\n", 1);
		close (0);
	}
}

void	set_signal_here_doc(void)
{
	struct sigaction	sa;
	struct sigaction 	sa2;

	ft_bzero(&sa, sizeof(struct sigaction));
	ft_bzero(&sa2, sizeof(struct sigaction));
	sa.sa_handler = &signal_here_doc;
	sa2.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa2, NULL);
}

int	open_here_doc(char **lex, t_commands *cmd, char **env, int *exit_status)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (cmd && lex[x][0] != T_NEWLINE)
	{
		if (lex[x][0] == LESSLESS)
		{
			lex[x + 1] = here_doc_infile(&lex[x], &cmd[i], env, exit_status);
			if (g_sig_flag || !lex[x + 1])
			{
				*exit_status = g_sig_flag;
				g_sig_flag = 0;
				return (1);
			}
			x += 2;
		}
		if (lex[x][0] == PIPE)
			i++;
		if (lex[x][0] != T_NEWLINE)
			x++;
	}
	return (0);
}

t_commands  *parsing(char **lex, char **env, int size, int *exit_status)
{
	t_commands *cmd;
	
	// if (!size)
	// 	return (NULL);
	cmd = create_struct_cmd(lex, env, size, exit_status);
	return (cmd);
}
