#include "minishell.h"

void	exit_after_failed_exec(t_cmd *cmd, t_shell *sh)
{
	sh = free_shell_null(sh);
	print_exec_error(cmd);
	cmd = free_lst_null(cmd);
	if (errno == 13)
		exit(126);
	if (errno == 2)
		exit(127);
	exit(127);
}

void	exit_on_file_error(t_cmd *cmd, t_shell *sh)
{
	cmd = free_lst_null(cmd);
	sh = free_shell_null(sh);
	exit (1);
}	
