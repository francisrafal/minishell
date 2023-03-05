#include "minishell.h"

int	child_process_single_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_path;
	char	**envp;

	if (cmd->fd_in == -1)
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit (1);
	}
	if (dup2_or_print_error(cmd->fd_in, STDIN_FILENO) == -1)
		return (-1);
	if (cmd->re_in)
		close_or_print_error(cmd->fd_in);
	if (dup2_or_print_error(cmd->fd_out, STDOUT_FILENO) == -1)
		return (-1);
	if (cmd->re_out)
		close_or_print_error(cmd->fd_out);
	if (is_builtin(cmd))
	{
		g_exit_code = exec_builtin(cmd, sh, EXEC_AS_CHILD);
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(g_exit_code);
	}
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

void	*exec_one_child(t_cmd *cmd, t_shell *sh)
{
	pid_t	pid;

	append_str(&cmd->path, "/");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (cmd);
	}
	if (pid == 0)
	{
		if (child_process_single_cmd(cmd, sh) == -1)
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
	}
	else
	{
		if (cmd->re_in)
			close_or_print_error(cmd->fd_in);
		if (cmd->re_out)
			close_or_print_error(cmd->fd_out);
		wait(&sh->wstatus);
		if (WIFEXITED(sh->wstatus))
			g_exit_code = WEXITSTATUS(sh->wstatus);
		unlink_heredoc(cmd);
	}
	return (cmd);
}