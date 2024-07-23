/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:29:32 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 18:00:06 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

int			less_infile(char **lex, t_cmd *cmd, int *ex_st);
int			great_outfile(char **lex, t_cmd *cmd, int *ex_st);
int			great_great_outfile(char **lex, t_cmd *cmd, int *ex_st);
int			is_an_argument(char **lex, t_cmd *cmd);

void		error_limiter_hd(char *limiteur);
void		error_ambiguous_redirect(int *ex_st);
void		error_open_file(char *str, int *ex_st);
t_cmd		*error_struct_cmd(t_cmd *cmd, int size, int *ex_st);

char		*here_doc_name(void);
int			write_here_doc(char **lex, char **env, int *ex_st, char *hd_name);
char		*here_doc_infile(char **lex, char **env, int *ex_st);
int			open_here_doc(char **lex, t_cmd *cmd, char **env, int *ex_st);

int			piped(t_cmd *cmd_in, t_cmd *cmd_out);
t_cmd		*fill_the_struct(char **lex, t_cmd *cmd, int size, int *ex_st);
t_cmd		*create_struct_cmd(char **lex, char **env, int size, int *ex_st);
t_cmd		*parsing(char **lex, char **env, int size, int *ex_st);

void		signal_here_doc(int signal);
void		set_signal_here_doc(void);

int			check_quote(char *limiter);
int			count_cmd(char **lex);
int			nb_of_arg(char **lex, int x);
int			set_struct_to_default(t_cmd *cmd, char **env, int size);
int			until_pipe_or_newline(char **lex);

#endif
