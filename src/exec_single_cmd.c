#include "minishell.h"

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

void	*exec_one_child(t_cmd *cmd, t_shell *sh)
{
	if (init_pid(sh, cmd->ncmds) == NULL)
		return (cmd);
	append_str(&cmd->path, "/");
	sh->pid[0] = fork_or_print_error();
	if (sh->pid[0] == -1)
		return (cmd);
	if (sh->pid[0] == 0)
	{
		if (child_process_single_cmd(cmd, sh) == -1)
			exit_after_failed_exec(cmd, sh);
	}
	else
	{
		if (cmd->re_in)
			close_or_print_error(cmd->fd_in);
		if (cmd->re_out)
			close_or_print_error(cmd->fd_out);
		sh->pid = wait_for_children(sh, 1);
		unlink_heredoc(cmd);
	}
	return (cmd);
}

int	child_process_single_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_path;
	char	**envp;

	if (cmd->fd_in == -1)
		exit_on_file_error(cmd, sh);
	if (dup2_or_print_error(cmd->fd_in, STDIN_FILENO) == -1)
		return (-1);
	if (cmd->re_in)
		close_or_print_error(cmd->fd_in);
	if (dup2_or_print_error(cmd->fd_out, STDOUT_FILENO) == -1)
		return (-1);
	if (cmd->re_out)
		close_or_print_error(cmd->fd_out);
	if (is_builtin(cmd))
		exec_builtin(cmd, sh, EXEC_AS_CHILD);
	else
	{
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
	}
	return (-1);
}
