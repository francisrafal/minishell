/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:46:22 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 13:41:57 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_fds_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh)
{
	if (cmd->fd_in == -1)
		exit_on_file_error(cmd, sh);
	if (cmd->next != NULL)
	{
		if (dup2_or_print_error(pipefd[1], STDOUT_FILENO) == -1)
			return (-1);
		close_or_print_error(pipefd[1]);
		close_or_print_error(pipefd[0]);
	}
	if (dup2_or_print_error(cmd->fd_in, STDIN_FILENO) == -1)
		return (-1);
	if (cmd->re_in || cmd->read_in)
		close_or_print_error(cmd->fd_in);
	if (dup2_or_print_error(cmd->fd_out, STDOUT_FILENO) == -1)
		return (-1);
	if (cmd->re_out)
		close_or_print_error(cmd->fd_out);
	close_or_print_error(sh->stdin_copy);
	return (0);
}

int	child_process_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh)
{
	if (redirect_fds_pipeline(pipefd, cmd, sh) == -1)
		return (-1);
	if (is_builtin(cmd))
		exec_builtin(cmd, sh, EXEC_AS_CHILD);
	else
		execve_safe(cmd, sh);
	return (-1);
}

int	parent_process_pipeline(int *pipefd, t_cmd *cmd)
{
	if (cmd->re_in || cmd->read_in)
		close_or_print_error(cmd->fd_in);
	if (cmd->re_out)
		close_or_print_error(cmd->fd_out);
	if (cmd->next != NULL)
	{
		close_or_print_error(pipefd[1]);
		if (dup2_or_print_error(pipefd[0], STDIN_FILENO) == -1)
		{
			unlink_heredoc(cmd);
			cmd = free_lst_null(cmd);
			return (-1);
		}
		close_or_print_error(pipefd[0]);
	}
	return (0);
}

int	pipeline_loop(int *pipefd, t_cmd *cmd, t_shell *sh, int i)
{
	append_str(&cmd->path, "/");
	if (cmd->next != NULL)
	{
		if (pipe_or_print_error(pipefd) == -1)
			return (-1);
	}
	sh->pid[i] = fork_or_print_error();
	if (sh->pid[i] == -1)
		return (-1);
	if (sh->pid[i] == 0)
	{
		if (child_process_pipeline(pipefd, cmd, sh) == -1)
			exit_after_failed_exec(cmd, sh);
	}
	else
	{
		if (parent_process_pipeline(pipefd, cmd) == -1)
			return (-2);
	}
	return (0);
}

void	*exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	int		pipefd[2];
	int		i;
	int		error;
	t_cmd	*next;

	if (init_pid(sh, cmd->ncmds) == NULL)
		return (cmd);
	sh->stdin_copy = dup(STDIN_FILENO);
	i = 0;
	while (cmd)
	{
		error = pipeline_loop(pipefd, cmd, sh, i++);
		if (error == -1)
			return (cmd);
		if (error == -2)
			break ;
		next = cmd->next;
		unlink_heredoc(cmd);
		cmd = free_cmd_null(cmd);
		cmd = next;
	}
	dup2_or_print_error(sh->stdin_copy, STDIN_FILENO);
	close_or_print_error(sh->stdin_copy);
	sh->pid = wait_for_children(sh, i);
	return (cmd);
}
