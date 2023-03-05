#include "minishell.h"

void	append_str(char ***paths, char *str)
{
	int		i;
	char	*tmp;

	tmp = NULL;
	i = 0;
	while ((*paths)[i] != NULL)
	{
		tmp = (*paths)[i];
		(*paths)[i] = ft_strjoin(tmp, str);
		tmp = free_null(tmp);
		i++;
	}
}

char	*get_cmd_path(t_cmd *cmd)
{
	char	*cmd_path;
	int		i;

	cmd_path = NULL;
	i = 0;
	if (cmd->path[i] == NULL)
		errno = 2;
	while ((cmd->path)[i] != NULL)
	{
		cmd_path = ft_strjoin(cmd->path[i], cmd->opt[0]);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		cmd_path = free_null(cmd_path);
		i++;
	}
	return (cmd_path);
}

int	child_process_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh)
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
	if (cmd->re_in)
		close_or_print_error(cmd->fd_in);
	if (dup2_or_print_error(cmd->fd_out, STDOUT_FILENO) == -1)
		return (-1);
	if (cmd->re_out)
		close_or_print_error(cmd->fd_out);
	close_or_print_error(sh->stdin_copy);
	if (is_builtin(cmd))
		exec_builtin(cmd, sh, EXEC_AS_CHILD);
	else
		execve_safe(cmd, sh);
	return (-1);
}

void	*exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	int		pipefd[2];
	int		i;
	t_cmd	*next;

	if (init_pid(sh, cmd->ncmds) == NULL)
		return (cmd);
	sh->stdin_copy = dup(STDIN_FILENO);
	i = 0;
	while (cmd)
	{
		append_str(&cmd->path, "/");
		if (cmd->next != NULL)
		{
			if (pipe_or_print_error(pipefd) == -1)
				return (cmd);
		}
		sh->pid[i] = fork_or_print_error();
		if (sh->pid[i] == -1)
			return (cmd);
		if (sh->pid[i] == 0)
		{
			if (child_process_pipeline(pipefd, cmd, sh) == -1)
				exit_after_failed_exec(cmd, sh);
		}
		else
		{
			if (cmd->re_in)
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
					break ;
				}
				close_or_print_error(pipefd[0]);
			}
			next = cmd->next;
			cmd->next = NULL;
			unlink_heredoc(cmd);
			cmd = free_lst_null(cmd);
			cmd = next;
			i++;
		}
	}
	dup2_or_print_error(sh->stdin_copy, STDIN_FILENO);
	close_or_print_error(sh->stdin_copy);
	sh->pid = wait_for_children(sh, i);
	return (cmd);
}
