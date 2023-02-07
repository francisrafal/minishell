#include "minishell.h"

void	free_data(t_shell *sh)
{
	free_arr(sh->env);
	free_null(sh);
}
