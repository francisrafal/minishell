#include "minishell.h"

void	handle_signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_exit_code = 130;
	}
}	

void	handle_signal_child(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_exit_code = 130;
	}
	if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		g_exit_code = 131;
	}
}	

void	handle_signal_builtin(int signum)
{
	(void)signum;
	ft_putstr_fd("SIGPIPE handled\n", STDERR_FILENO);
}

void	set_signal_action(int mode)
{
	if (mode == EXEC_AS_PARENT)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_signal_parent);
	}
	if (mode == EXEC_AS_CHILD)
	{
		signal(SIGINT, handle_signal_child);
		signal(SIGQUIT, handle_signal_child);
	}
	if (mode == EXEC_BUILTIN)
	{
		signal(SIGPIPE, handle_signal_builtin);
	}
}
