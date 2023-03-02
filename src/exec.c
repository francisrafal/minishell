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
	{
		set_signal_action(EXEC_AS_CHILD);
		exec_one_child(cmd, sh);
	}
}
