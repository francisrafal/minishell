#include "minishell.h"

int	exec_builtin(char **cmd_args, t_shell *sh, int mode)
{
	if (ft_strncmp(cmd_args[0], "echo", 5) == 0)
		g_exit_code = bi_echo(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "pwd", 4) == 0)
		g_exit_code = bi_pwd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		g_exit_code = bi_cd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		g_exit_code = bi_exit(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "env", 4) == 0)
		g_exit_code = bi_env(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "export", 7) == 0)
		g_exit_code = bi_export(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "unset", 6) == 0)
		g_exit_code = bi_unset(cmd_args, sh, mode);
	return (g_exit_code);
}

/* Maybe remove this later */
void	exec_bi_as_child(char **cmd_args, t_shell *sh)
{
	if (fork() == 0)
	{
		g_exit_code = exec_builtin(cmd_args, sh, EXEC_AS_CHILD);
		free_arr(cmd_args);
		free_data(sh);
		exit(g_exit_code);
	}
	else
	{	
		// Replace here with waitpid when we have multiple cmds
		wait(&sh->wstatus);
		if (WIFEXITED(sh->wstatus))
			g_exit_code = WEXITSTATUS(sh->wstatus);
		// Later: EXIT_NO_ARG is used so that $? is set properly. if EXIT_NO_ARG then $? should remain the value of the last pipeline
		if (WEXITSTATUS(sh->wstatus) == EXIT_NO_ARG)
			sh->exit = 1;
	}
}

void	exec_single_cmd(t_cmd *cmd, t_shell *sh)
{
	if (ft_strncmp(cmd->opt[0], "cd", 3) == 0)
		exec_builtin(cmd->opt, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "exit", 5) == 0)
		exec_builtin(cmd->opt, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "export", 7) == 0)
		exec_builtin(cmd->opt, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "unset", 6) == 0)
		exec_builtin(cmd->opt, sh, EXEC_AS_PARENT);
	else
		exec_one_child(cmd, sh);
}
