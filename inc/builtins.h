#ifndef BUILTINS_H
# define BUILTINS_H

char	**ft_cd(t_commands *cmd, int *exit_status);
char	**switch_pwd_env(char **env);
char	**update_pwd(char **env);
char	*join_with_new_pwd(char *str);
char	**put_pwd_in_old_pwd(char **env);
char	*join_with_pwd(char **env, char *str);
char	*troncate_equal(char *str);
int	count_arg(char **arg);
void	free_tab(char **tab);

int	ft_echo(t_commands *cmd, int *exit_status);
void	print_with_new(char **arg);
void	print_without_new(char **arg);
int	is_n_option(char *arg);

int	ft_env(t_commands *cmd, int *exit_status);

char    **ft_export(t_commands *cmd, int size, int *exit_status);
int	print_sort_env(char **env);
char	**add_env_variable(t_commands *cmd, int *exit_status);
char	**add_arg_to_env(char *str, char **env, int *exit_status);
char **create_arg(char *str, char **env_tmp);
char	*create_without_plus(char *str);
char	**realloc_and_copy_env(char **env_tmp, int size);
char *replace_with_arg(char *str, char *env);
char	*join_arg(char *str, char *env);
char *replace(char *str, char *env);
int search_arg_in_env(char *str, char *env);
int check_argument_export(char *str, int *exit_status);
int	error_arg_export(char *str, int *exit_status);
void	print_tab_int(int *tab, int size, char **env);
void	print_env_value(char *str);
int	cmp_env(char *s1, char *s2);

int	ft_pwd(void);

char	**ft_unset(t_commands *cmd, int *exit_status);
char	**try_to_unset(t_commands *cmd, int x, int *exit_status);
char	**erase_env(char **env_tmp, int i);

# endif