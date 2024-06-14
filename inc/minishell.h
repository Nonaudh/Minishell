#ifndef MINISHELL_H
# define MINISHELL_H

#include "../lib/libft/libft.h"


#include <fcntl.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token
{
	GREAT = 1,
	LESS,
	GREATGREAT,
	LESSLESS,
	PIPE,
	T_NEWLINE,
} t_token;

typedef struct s_commands
{
	char **arg;
	char **env;
	int fd_in;
	int fd_out;
} t_commands;


int 	main(int argc, char **argv, char **env_tmp);
int		minishell(char *line, char **env, int exit_status);

char	**lexing(char *line, char **env, int exit_status);
int		count_argc(char *str);
char	**fill_lex(char *line, int argc);
char	*next_argv(char *line, int *x);

char	*return_token(char *str, int *x);
char	*ft_ttoa(t_token token);
char	*create_token(t_token token);
int		check_syntax(char **lex);

char	**expand_lex(char **lex, char **env, int exit_status);
char	*expand_env(char *str, char **env, int exit_status);
char	*add_env_value(char *str, int size, char **env, int exit_status);
char	*unquote(char *str);

void	print_tab(char **tab);
void	free_the_tab(char **tab);
char	**ft_str_tab_dup(char **tab1);
char	*ft_strjoin_dup_free_s1(char const *s1, char const *s2);
char	*ft_strjoin_dup_frees(char const *s1, char const *s2);
int 	ft_strchr_index(char const *str, int c);

int	is_a_char_token(char c);
int	is_a_token(t_token token);
int	env_variable_detected(char *str);



t_commands  *parsing(char **lex, char **env);

# endif
