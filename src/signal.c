#include "../inc/minishell.h"

// int sig_flag = 0;

// void	signal_here_doc_test(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		sig_flag = 1;
// 		ft_putstr_fd("\n", 1);
// 		//rl_on_new_line();
// 		// rl_replace_line(NULL, 0);
// 		// rl_redisplay();
// 		close (0);
// 	}
// }

// void	set_signal_here_doc_test(void)
// {
// 	struct sigaction	sa;

// 	ft_bzero(&sa, sizeof(struct sigaction));
// 	sa.sa_handler = &signal_here_doc_test;
// 	sigaction(SIGINT, &sa, NULL);
// }

// int main(int argc, char **argv, char **env)
// {
// 	char	*line;
// 	int fd_hd;
// 	int std_in;

// 	std_in = dup(STDIN_FILENO);
// 	set_signal_here_doc_test();
// 	if (argc != 2)
// 		return (1);
// 	fd_hd = open("/tmp/here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (fd_hd == -1)
// 		perror("here_doc");

// 	line = readline(">");	
// 	while (line && (ft_strlen(line) != ft_strlen(argv[1]) 
// 			|| !ft_strnstr(line, argv[1], ft_strlen(argv[1]))))
// 	{
// 		ft_putendl_fd(line, fd_hd);
// 		free(line);
// 		line = readline(">");
// 	}
// 	if (!line && !sig_flag)
// 		printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n", argv[1]);
// 	free(line);
// 	close(fd_hd);
// 	if (sig_flag)
// 		dup2(std_in, STDIN_FILENO);
// 	close(std_in);
// 	return (0);
// }

// // int	here_doc_infile(char **lex, t_commands *cmd, char **env, int exit_status)
// // {
// // 	write_here_doc(&lex[1], env, exit_status);
// // 	if (cmd->fd_in != STDIN_FILENO)
// // 		close(cmd->fd_in);
// // 	cmd->fd_in = open("/tmp/here_doc", O_RDONLY);
// // 	if (cmd->fd_in == -1)
// // 		perror("here_doc_infile");
// // 	else if (unlink("/tmp/here_doc"))
// // 		printf("Error unlink infile\n");
// // 	return (2);
// // }

void	print_tab(char **tab)
{
	int i = 0;

	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

void	free_the_tab(char **tab)
{
	int i = 0;

	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_str_tab_dup_without(char **tab1, int x)
{
	int	i = 0;
	char	**tab2;

	if (!tab1)
		return (NULL);
	while (tab1[i])
		i++;
	tab2 = malloc(sizeof(char *) * (i + 1));
	if (!tab2)
		return (NULL);
	i = 0;
	while (tab1[i])
	{
		if (i == x)
			tab1++;
		tab2[i] = ft_strdup(tab1[i]);
		if (!tab2[i])
			return (NULL);
		i++;
	}
	tab2[i] = NULL;
	return (tab2);
}
int	erase_path(char ***env)
{
	int i = 0;
	char **new_env;

	while (env[0][i] && !ft_strnstr(env[0][i], "PATH=", 5))
		i++;
	if (env[0][i])
		new_env = ft_str_tab_dup_without(*env, i);
	else
		printf("not found\n");
	free_the_tab(*env);
	*env = new_env;
	return (0);
}

typedef struct s_cmd
{
	char ***env;
}	t_cmd;

char	**copy_str (char*str)
{

}

char	*fill_name(void)
{
	char	name[10][20];

	ft_strlcpy(name[0], "tmp/here_doc", 20);
	return (&name[0][0]);
}

int main(int argc, char **argv, char **env_tmp)
{
	// char	**env;
	// t_cmd cmd;

	// env = ft_str_tab_dup_without(env_tmp, -1);
	// cmd.env = &env;
	// erase_path(cmd.env);
	// print_tab(env);
	// free_the_tab(env);

	char	*name;

	name = fill_name();
}