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
	if (mode == EXEC_AS_CHILD)
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(g_exit_code);
	}
	return (g_exit_code);
}

void	*exec_single_cmd(t_cmd *cmd, t_shell *sh)
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
		cmd = exec_one_child(cmd, sh);
	}
	return (cmd);
}

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
