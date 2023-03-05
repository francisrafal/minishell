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

int	is_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->opt[0], "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd->opt[0], "unset", 6) == 0)
		return (1);
	return (0);
}

t_cmd	*exec_parsed_cmd_line(t_cmd *cmd, t_shell *sh)
{
	if (cmd->ncmds == 1)
		cmd = exec_single_cmd(cmd, sh);
	else
	{
		set_signal_action(EXEC_AS_CHILD);
		cmd = exec_pipeline(cmd, sh);
	}
	cmd = free_lst_null(cmd);
	return (cmd);
}
