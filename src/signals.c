#include "minishell.h"

void	signal_handler_int(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", STDERR_FILENO);
}	

void	signal_handler_quit(int signum)
{
	(void)signum;
	/*
	ft_putstr_fd("\b\b \b\b", STDERR_FILENO);
	*/
	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
}	
