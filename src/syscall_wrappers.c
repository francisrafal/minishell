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