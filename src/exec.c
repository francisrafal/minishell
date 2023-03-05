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

int	execve_safe(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_path;
	char	**envp;

	if (cmd->opt[0][0] == '\0')
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit (0);
	}
	if (cmd->opt[0] - ft_strchr(cmd->opt[0], '/') == (long)cmd->opt[0])
	{
		cmd_path = get_cmd_path(cmd);
		if (cmd_path == NULL)
			return (-1);
	}
	else
		cmd_path = ft_strdup(cmd->opt[0]);
	envp = get_env_arr(sh->env);
	sh = free_shell_null(sh);
	if (execve(cmd_path, cmd->opt, envp) == -1)
	{
		cmd_path = free_null(cmd_path);
		envp = free_arr_null(envp);
	}
	return (-1);
}
