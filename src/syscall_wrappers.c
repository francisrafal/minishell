#include "minishell.h"

void	close_or_print_error(int fd)
{
	if (close(fd) == -1)
	{
		perror("close");
		ft_putstr_fd("\n", 2);
		ft_putnbr_fd(getpid(), 2);
		ft_putstr_fd("\n", 2);
		ft_putnbr_fd(fd, 2);
		ft_putstr_fd("\n", 2);
	}
}

int	dup2_or_print_error(int oldfd, int newfd)
{
	int	fd;

	fd = dup2(oldfd, newfd);
	if (fd == -1)
		perror("dup2");
	return (fd);
}

void	unlink_heredoc(t_cmd *cmd)
{
	char	*name;

	if (!cmd->read_in)
		return ;
	name = get_heredoc_name(cmd->cmd_id);
	if (unlink(name) == -1)
		perror("unlink");
	name = free_null(name);
}
