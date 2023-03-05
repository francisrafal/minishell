/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:46:34 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 13:54:41 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	redirect_fds_single_cmd(t_cmd *cmd, t_shell *sh)
{
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
	return (0);
}

int	child_process_single_cmd(t_cmd *cmd, t_shell *sh)
{
	if (redirect_fds_single_cmd(cmd, sh) == -1)
		return (-1);
	if (is_builtin(cmd))
		exec_builtin(cmd, sh, EXEC_AS_CHILD);
	else
		execve_safe(cmd, sh);
	return (-1);
}
