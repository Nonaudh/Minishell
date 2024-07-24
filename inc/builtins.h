/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:59:36 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:40 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

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

char	**ft_cd(t_cmd *cmd, int size, int *ex_st);
char	**switch_pwd_env(char **env);
char	**update_pwd(char **env);
char	*join_with_new_pwd(char *str);
char	**put_pwd_in_old_pwd(char **env);
char	*join_with_pwd(char **env, char *str);
char	*troncate_equal(char *str);
int		count_arg(char **arg);
void	free_tab(char **tab);
void	error_no_such_file(char *arg, int *ex_st);
void	error(char *str, int *ex_st);

int		ft_echo(t_cmd *cmd, int *ex_st);
void	print_with_new(char **arg);
void	print_without_new(char **arg);
int		is_n_option(char *arg);

int		ft_env(t_cmd *cmd, int *ex_st);

char	**ft_exit(t_cmd *cmd, int *ex_st);
int		positive_exit(int nb);
int		not_numerical_value(char *arg);
int		negative_exit(int nb);

char	**ft_export(t_cmd *cmd, int size, int *ex_st);
int		print_sort_env(char **env, int size_env);
char	**add_env_variable(t_cmd *cmd, int size, int *ex_st);
char	**add_arg_to_env(char *str, char **env, int size, int *ex_st);
char	**create_arg(char *str, char **env_tmp);
char	*create_without_plus(char *str);
char	**realloc_and_copy_env(char **env_tmp, int size);
char	*replace_with_arg(char *str, char *env);
char	*join_arg(char *str, char *env);
char	*replace(char *str, char *env);
int		search_arg_in_env(char *str, char *env);
int		check_argument_export(char *str, int *ex_st);
int		error_arg_export(char *str, int *ex_st);
void	print_tab_int(int *tab, int size, char **env);
void	print_env_value(char *str);
int		cmp_env(char *s1, char *s2);

int		ft_pwd(t_cmd *cmd, int *ex_st);

char	**ft_unset(t_cmd *cmd, int size, int *ex_st);
char	**try_to_unset(t_cmd *cmd, int x);
char	**erase_env(char **env_tmp, int i);

char	**error_option(t_cmd *cmd, int *ex_st, int error_code);
int		check_option(char **arg);

#endif
