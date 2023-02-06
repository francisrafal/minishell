/*TO DO: I will use this from pipex with the new struct t_cmd*/


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
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf) < 0)
			exit(1);
		if (!ft_strncmp(av, buf, ft_strlen(av)))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	file = open(".heredoc_tmp", O_RDONLY, 0644);
	return (file);
}

int	get_infile(char **av)
{
	int	fd_in;

	if (ft_strncmp("here_doc", av[1], 9))
	{
		fd_in = open(av[1], O_RDONLY);
		if (fd_in < 0)
			ft_error(strerror(errno), av[1]);
		return (fd_in);
	}
	else
	{
		fd_in = here_doc(av[2]);
		if (fd_in < 0)
			ft_error(strerror(errno), av[1]);
		return (fd_in);
	}
}

int	get_outfile(int ac, char **av)
{
	int	fd_out;

	if (ft_strncmp("here_doc", av[1], 9))
	{
		fd_out = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		return (fd_out);
	}
	else
	{
		fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		return (fd_out);
	}
}
