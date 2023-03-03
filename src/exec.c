#include "minishell.h"

int	exec_builtin(t_cmd *cmd, t_shell *sh, int mode)
{
	if (ft_strncmp(cmd->opt[0], "echo", 5) == 0)
		g_exit_code = bi_echo(cmd->opt, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "pwd", 4) == 0)
		g_exit_code = bi_pwd(cmd->opt, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "cd", 3) == 0)
		g_exit_code = bi_cd(cmd->opt, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "exit", 5) == 0)
		g_exit_code = bi_exit(cmd, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "env", 4) == 0)
		g_exit_code = bi_env(cmd->opt, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "export", 7) == 0)
		g_exit_code = bi_export(cmd->opt, sh, mode);
	else if (ft_strncmp(cmd->opt[0], "unset", 6) == 0)
		g_exit_code = bi_unset(cmd->opt, sh, mode);
	return (g_exit_code);
}

void	exec_single_cmd(t_cmd *cmd, t_shell *sh)
{
	if (ft_strncmp(cmd->opt[0], "cd", 3) == 0)
		exec_builtin(cmd, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "exit", 5) == 0)
		exec_builtin(cmd, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "export", 7) == 0)
		exec_builtin(cmd, sh, EXEC_AS_PARENT);
	else if (ft_strncmp(cmd->opt[0], "unset", 6) == 0)
		exec_builtin(cmd, sh, EXEC_AS_PARENT);
	else
	{
		set_signal_action(EXEC_AS_CHILD);
		exec_one_child(cmd, sh);
	}
}
