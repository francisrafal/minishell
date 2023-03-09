/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:47:27 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:52:14 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_heredoc_name(int cmd_id)
{
	char	*nb;
	char	*name;

	name = NULL;
	nb = ft_itoa(cmd_id);
	name = ft_strjoin(".heredoc_tmp", nb);
	if (!name)
		return (NULL);
	nb = free_null(nb);
	return (name);
}

void	write_heredoc(int file, char *av)
{
	char	*buf;

	while (1)
	{
		buf = readline(HERE_DOC);
		if (buf == NULL)
		{
			write(2, "minishell: warning : here-document delimited ", 45);
			write(2, "by end-of-file (wanted `", 24);
			write(2, av, ft_strlen(av));
			write(2, "`)\n", 3);
			break ;
		}
		if (!ft_strncmp(av, buf, ft_strlen(av)))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		buf = free_null(buf);
	}
	buf = free_null(buf);
}

int	here_doc(char *av, t_cmd *cmd)
{
	int		file;
	char	*name;

	name = get_heredoc_name(cmd->cmd_id);
	if (!name)
		return (-1);
	file = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
		return (ft_error(strerror(errno), ".heredoc_tmp"));
	write_heredoc(file, av);
	close_or_print_error(file);
	file = open(name, O_RDONLY, 0644);
	name = free_null(name);
	return (file);
}

int	handle_infile(char *tmp, char *file, t_cmd *cmd)
{
	int	fd_in;

	if (tmp[1] == '<')
	{
		cmd->read_in = 1;
		cmd->re_in = 0;
		fd_in = here_doc(file, cmd);
	}
	else
	{
		cmd->read_in = 0;
		cmd->re_in = 1;
		fd_in = open(file, O_RDONLY);
		if (fd_in < 0)
			ft_error(strerror(errno), file);
	}
	return (fd_in);
}

char	*get_infile(t_cmd *cmd, char *line)
{
	int		fd_in;
	char	*tmp;
	char	*file;

	file = NULL;
	tmp = get_strchroq(line, '<');
	if (!tmp)
		return (line);
	while (tmp)
	{
		file = get_file_name(line, '<');
		fd_in = handle_infile(tmp, file, cmd);
		line = cut_word(line, '<');
		tmp = get_strchroq(line, '<');
		if (tmp)
		{
			if (fd_in)
				close_or_print_error(fd_in);
			unlink_heredoc(cmd);
			file = free_null(file);
		}
	}
	cmd->fd_in = fd_in;
	file = free_null(file);
	return (line);
}
