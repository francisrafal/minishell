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

int	child_process_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh)
{
	char	*cmd_path;
	char	**envp;

	if (cmd->fd_in == -1)
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit (1);
	}	
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

void	*exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	int		pipefd[2];
	int		i;
	int		j;
	t_cmd	*next;

	sh->pid = malloc(sizeof (pid_t) * cmd->ncmds);
	if (sh->pid == NULL)
		return (cmd);
	sh->stdin_copy = dup(STDIN_FILENO);
	i = 0;
	while (cmd)
	{
		append_str(&cmd->path, "/");
		if (cmd->next != NULL)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return (cmd);
			}
		}
		sh->pid[i] = fork();
		if (sh->pid[i] == -1)
		{
			perror("fork");
			return (cmd);
		}
		if (sh->pid[i] == 0)
		{
			if (child_process_pipeline(pipefd, cmd, sh) == -1)
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
	j = 0;
	while (j < i)
	{
		if (waitpid(sh->pid[j], &sh->wstatus, 0) == -1)
			perror("waitpid");
		j++;
	}
	if (WIFEXITED(sh->wstatus))
	{
		if (WEXITSTATUS(sh->wstatus) != EXIT_NO_ARG)
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	sh->pid = free_null(sh->pid);
	return (cmd);
}
