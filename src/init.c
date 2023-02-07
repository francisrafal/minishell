#include "minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*sh;

	sh = malloc(sizeof (t_shell));
	if (sh == NULL)
		perror_exit("malloc");
	sh->env = copy_env(envp);
	sh->exit = 0;
	return (sh);
}
