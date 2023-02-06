#include "minishell.h"

void	perror_exit(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

