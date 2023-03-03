#include "minishell.h"

int	here_doc(char *av)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
		return (ft_error(strerror(errno), ".heredoc_tmp"));
	while (1)
	{
		buf = readline(HERE_DOC);
		if (!ft_strncmp(av, buf, ft_strlen(av)))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		buf = free_null(buf);
	}
	buf = free_null(buf);
	close(file);
	file = open(".heredoc_tmp", O_RDONLY, 0644);
	return (file);
}

int	handle_infile(char *tmp, char *file, t_cmd *cmd)
{
	int	fd_in;

	if (tmp[1] == '<')
	{
		cmd->read_in = 1;
		cmd->re_in = 0;
		fd_in = here_doc(file);
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
	tmp = ft_strchr(line, '<');
	if (!tmp)
		return (line);
	while (tmp)
	{
		file = get_file_name(line, '<');
		fd_in = handle_infile(tmp, file, cmd);
		line = cut_word(line, '<');
		tmp = ft_strchr(line, '<');
		if (tmp)
		{
			if (fd_in)
				close(fd_in);
			file = free_null(file);
		}
	}
	cmd->fd_in = fd_in;
	file = free_null(file);
	return (line);
}
