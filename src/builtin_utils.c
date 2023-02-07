#include "minishell.h"

int	calc_argc(char **cmd_args)
{
	int argc;

	argc = 0;
	while (cmd_args[argc] != NULL)
		argc++;
	return (argc);
}
