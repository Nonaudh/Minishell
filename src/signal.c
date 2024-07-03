#include "../inc/minishell.h"

int sig_flag = 0;

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		sig_flag = 1;
		ft_putstr_fd("\n", 1);
		//rl_on_new_line();
		// rl_replace_line(NULL, 0);
		// rl_redisplay();
		close (0);
	}
}

void	set_signal_action(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = &sigint_handler;
	sigaction(SIGINT, &sa, NULL);
}

int main(int argc, char **argv, char **env)
{
	char	*line;
	int fd_hd;
	int std_in;

	std_in = dup(STDIN_FILENO);
	set_signal_action();
	if (argc != 2)
		return (1);
	fd_hd = open("/tmp/here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_hd == -1)
		perror("here_doc");

	line = readline(">");	
	while (line && (ft_strlen(line) != ft_strlen(argv[1]) 
			|| !ft_strnstr(line, argv[1], ft_strlen(argv[1]))))
	{
		ft_putendl_fd(line, fd_hd);
		free(line);
		line = readline(">");
	}
	if (!line && !sig_flag)
		printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", argv[1]);
	free(line);
	close(fd_hd);
	if (sig_flag)
		dup2(std_in, STDIN_FILENO);
	close(std_in);
	return (0);
}

// int	here_doc_infile(char **lex, t_commands *cmd, char **env, int exit_status)
// {
// 	write_here_doc(&lex[1], env, exit_status);
// 	if (cmd->fd_in != STDIN_FILENO)
// 		close(cmd->fd_in);
// 	cmd->fd_in = open("/tmp/here_doc", O_RDONLY);
// 	if (cmd->fd_in == -1)
// 		perror("here_doc_infile");
// 	else if (unlink("/tmp/here_doc"))
// 		printf("Error unlink infile\n");
// 	return (2);
// }
