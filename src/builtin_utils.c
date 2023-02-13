#include "minishell.h"

int	get_arr_size(char **arr)
{
	int argc;

	argc = 0;
	while (arr[argc] != NULL)
		argc++;
	return (argc);
}
