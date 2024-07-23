/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:59:57 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:59 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_commands
{
	char	**arg;
	char	**env;
	int		fd_in;
	int		fd_out;
}	t_cmd;

typedef enum e_token
{
	GREAT = 1,
	LESS,
	GREATGREAT,
	LESSLESS,
	PIPE,
	T_NEWLINE,
	T_EMPTY,
}	t_token;

# include "../lib/libft/libft.h"
# include "builtins.h"
# include "execution.h"
# include "parsing.h"
# include "lexing.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <errno.h>
# include <signal.h>

extern int	g_sig_flag;

int		main(int argc, char **argv, char **env_tmp);
char	**minishell(char *line, char **env, int *ex_st);

char	**lexing(char *line, char **env, int *ex_st);
int		count_argc(char *str);
char	**fill_lex(char *line, int argc);
char	*next_argv(char *line, int *x);

char	*return_token(char *str, int *x);
char	*ft_ttoa(t_token token);
char	*create_token(t_token token);
int		check_syntax(char **lex);

char	**expand_lex(char **lex, char **env, int *ex_st);
char	*expand_env(char *str, char **env, int *ex_st);
char	*add_env_value(char *little, char **env, int *ex_st);
char	*unquote(char *str);

void	print_tab(char **tab);
void	free_the_tab(char **tab);
char	**ft_str_tab_dup(char **tab1);
char	*ft_strjoin_dup_free_s1(char const *s1, char const *s2);
char	*ft_strjoin_dup_frees(char const *s1, char const *s2);
int		ft_strchr_index(char const *str, int c);

int		is_token(char c);
int		is_a_token(t_token token);
int		env_variable_detected(char *str);

t_cmd	*parsing(char **lex, char **env, int size, int *ex_st);
void	set_signal_here_doc(void);
int		open_here_doc(char **lex, t_cmd *cmd, char **env, int *ex_st);

int		count_cmd(char **lex);
int		end_of_env_var(char *str);
int		not_a_env_var(char *str);

char	**execution(t_cmd *cmd, int size, int *ex_st);
int		execute_command(t_cmd *cmd, int i, int size, int *ex_st);
char	*ft_getenv(char *str, char **env);
void	free_struct_cmd(t_cmd *cmd, int size);
void	error_open_file(char *str, int *ex_st);

void	set_signal_main(void);
void	handle_sig_main(int signal);

#endif
