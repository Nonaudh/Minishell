/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahuge <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:58:49 by ahuge             #+#    #+#             */
/*   Updated: 2024/07/23 17:58:51 by ahuge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*here_doc_name(void)
{
	if (access("/tmp/9036fddhsg", F_OK))
		return ("/tmp/9036fddhsg");
	if (access("/tmp/ie856fdgskd", F_OK))
		return ("/tmp/ie856fdgskd");
	if (access("/tmp/9ioqrac34g", F_OK))
		return ("/tmp/9ioqrac34g");
	if (access("/tmp/hfdyr649834hdf", F_OK))
		return ("/tmp/hfdyr649834hdf");
	return ("/tmp/jdhw782gtfsdh");
}

int	write_here_doc(char **lex, char **env, int *ex_st, char *hd_name)
{
	int		fd_hd;
	char	*line;
	int		quote;

	quote = check_quote(lex[0]);
	lex[0] = unquote(lex[0]);
	fd_hd = open(hd_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_hd == -1)
		perror("here_doc");
	line = readline(">");
	while (line && (ft_strlen(line) != ft_strlen(lex[0])
			|| !ft_strnstr(line, lex[0], ft_strlen(lex[0]))))
	{
		if (quote && env_variable_detected(line))
			line = expand_env(line, env, ex_st);
		ft_putendl_fd(line, fd_hd);
		free(line);
		line = readline(">");
	}
	if (!line && !g_sig_flag)
		error_limiter_hd(lex[0]);
	free(line);
	close(fd_hd);
	return (0);
}

char	*here_doc_infile(char **lex, char **env, int *ex_st)
{
	int		fdin_tmp;
	char	*hd_name;

	fdin_tmp = dup(STDIN_FILENO);
	set_signal_here_doc();
	hd_name = here_doc_name();
	write_here_doc(&lex[1], env, ex_st, hd_name);
	if (g_sig_flag)
	{
		dup2(fdin_tmp, STDIN_FILENO);
		if (unlink(hd_name))
			perror("unlike");
		close(fdin_tmp);
		return (lex[1]);
	}
	close(fdin_tmp);
	free(lex[1]);
	return (ft_strdup(hd_name));
}

int	open_here_doc(char **lex, t_cmd *cmd, char **env, int *ex_st)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (cmd && lex[x][0] != T_NEWLINE)
	{
		if (lex[x][0] == LESSLESS)
		{
			lex[x + 1] = here_doc_infile(&lex[x], env, ex_st);
			if (g_sig_flag || !lex[x + 1])
			{
				*ex_st = g_sig_flag;
				g_sig_flag = 0;
				return (1);
			}
			x += 2;
		}
		if (lex[x][0] == PIPE)
			i++;
		if (lex[x][0] != T_NEWLINE)
			x++;
	}
	return (0);
}
