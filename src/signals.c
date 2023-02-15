#include "minishell.h"

void	handle_sigint_parent(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_code = 130;
}	

void	handle_sigquit_child(int signum)
{
	(void)signum;
	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
}	

void	set_signal_action(int mode)
{
	if (mode == EXEC_AS_PARENT)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint_parent);
	}
	if (mode == EXEC_AS_CHILD)
	{

	}
}
