#include "minishell.h"

pid_t	*wait_for_children(t_shell *sh, int children)
{
	int	i;

	i = 0;
	while (i < children)
	{
		if (waitpid(sh->pid[i], &sh->wstatus, 0) == -1)
			perror("waitpid");
		i++;
	}
	if (WIFEXITED(sh->wstatus))
	{
		if (WEXITSTATUS(sh->wstatus) != EXIT_NO_ARG)
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	sh->pid = free_null(sh->pid);
	return (sh->pid);
}
