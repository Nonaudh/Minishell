/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:59:43 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:45 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

int		is_a_buildins(char **arg);
char	**execute_in_main_process(t_cmd *cmd, int size, int *ex_st);
void	ignore_sigpipe(void);
void	default_sigpipe(void);
char	**execute_builtins(t_cmd *cmd, int size, int *ex_st, int *fork_pid);

char	**create_paths_tab(t_cmd *cmd);
int		is_a_directory(t_cmd *cmd, int i, int size);
int		exec_cmd(t_cmd *cmd, int i, int size);
int		execute_in_child_process(t_cmd *cmd, int i, int size);
int		execute_command(t_cmd *cmd, int i, int size, int *ex_st);

int		close_all_fd_except(t_cmd *cmd, int i, int size);
int		wait_for_all_process(int fork_pid, int *ex_st);
char	**execution(t_cmd *cmd, int size, int *ex_st);

char	*ft_getenv(char *str, char **env);
void	error_is_a_directory(char *dir);
void	error_cmd_not_found(char *cmd);
int		ft_strcmp(char *s1, char *s2);

#endif
