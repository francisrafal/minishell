#include "minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*sh;

	sh = malloc(sizeof (t_shell));
	if (sh == NULL)
		perror_exit("malloc");
	sh->env = init_env(envp);
	sh->exit = 0;
	sh->last_exit_status = 0;
	return (sh);
}
