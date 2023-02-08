#include "minishell.h"

int	get_arr_size(char **cmd_args)
{
	int argc;

	argc = 0;
	while (cmd_args[argc] != NULL)
		argc++;
	return (argc);
}
