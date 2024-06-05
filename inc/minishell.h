#ifndef MINISHELL_H
# define MINISHELL_H

#include "../lib/libft/libft.h"

#include <stdio.h>
#include <fcntl.h>

typedef enum e_token
{
	GREAT = 1,
	LESS,
	GREATGREAT,
	LESSLESS,
	PIPE,
	NEWLINE,
} t_token;

int 	main(int argc, char **argv, char **env_tmp);
int		minishell(char *line, char **env);

char	**lexing(char *line, char **env);
int		count_argc(char *str);
char	**fill_lex(char *line, int argc);
char	*next_argv(char *line, int *x);

char	*return_token(char *str, int *x);
char	*ft_ttoa(t_token token);
char	*create_token(t_token token);
int		check_syntax(char **lex);

char	**expand_lex(char **lex, char **env);
char	*expand_env(char *str, char **env);
char	*add_env_value(char *str, int start, int i, char **env);
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

# endif
