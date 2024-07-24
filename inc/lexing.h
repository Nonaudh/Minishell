/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:59:02 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:59:52 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

char	*erase_quote_and_cpy(char *lex, char *str);
char	*unquote(char *str);
char	*add_env_value(char *little, char **env, int *ex_st);
char	*expand_env(char *str, char **env, int *ex_st);
char	**expand_lex(char **lex, char **env, int *ex_st);

char	*next_argv(char *line, int *x);
int		add_token_newline(char **lex);
char	**fill_lex(char *line, int argc);
int		count_argc(char *str);
char	**lexing(char *line, char **env, int *ex_st);

int		error_token(t_token token);
int		check_syntax(char **lex);
char	*create_token(t_token token);
char	*ft_ttoa(t_token token);
char	*return_token(char *str, int *x);

int		is_token(char c);
int		is_a_token(t_token token);
int		env_variable_detected(char *str);
int		not_a_env_var(char *str);
int		end_of_env_var(char *str);

int		error_quote(void);
void	free_the_tab(char **tab);
void	ugly_copy_plus_plus(char *src, char *dst, int *i);
int		count_plus_plus(char *str);

void	free_struct_cmd(t_cmd *cmd, int size);
char	**ft_str_tab_dup(char **tab1);
char	*ft_strjoin_dup_free_s1(char const *s1, char const *s2);
char	*ft_strjoin_dup_frees(char const *s1, char const *s2);
int		ft_strchr_index(char const *str, int c);

#endif
