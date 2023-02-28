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
		free(tmp);
		i++;
	}
}

char	*get_cmd_path(t_cmd *cmd)
{
	char	*cmd_path;
	int		i;

	cmd_path = NULL;
	i = 0;
	while ((cmd->path)[i] != NULL)
	{
		cmd_path = ft_strjoin(cmd->path[i], cmd->opt[0]);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	perror_exit("access");
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

int	child_process_pipeline(int *pipefd, t_cmd *cmd, char **envp, t_shell *sh)
{
	char	*cmd_path;
		/* Questionable here */
	char	**cmd_opt;
		/* Questionable end here */

	if (cmd->next != NULL)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2_1");
			return (-1);
		}
	}
	if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
	{
		perror("dup2_2");
		return (-1);
	}
	if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
	{
		perror("dup2_3");
		return (-1);
	}
	// handle Signals
	close(pipefd[0]);
	close(sh->stdin_copy);
	if (is_builtin(cmd))
	{
		g_exit_code = exec_builtin(cmd->opt, sh, EXEC_AS_CHILD);
		free_data(sh);
		/* Questionable here */
		free_null(cmd->delim);
		free_arr(cmd->path);
		free_arr(cmd->opt);
		free_null(cmd);
		free_arr(envp);
		/* Questionable end here */
		exit(g_exit_code);
	}
	else
	{
		cmd_path = get_cmd_path(cmd);
		free_data(sh);
		/* Questionable here */
		free_null(cmd->delim);
		free_arr(cmd->path);
		cmd_opt = cmd->opt;
		free_null(cmd);
		/* Questionable end here */
		// do for commands without forward slash first, then later handle relative and absolute paths
		if (execve(cmd_path, cmd_opt, envp) == -1)
			free(cmd_path);
	}
	return (-1);
}

void	exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	int		pipefd[2];
	char	**envp;
	pid_t	pid;

	envp = get_env_arr(sh->env);
	sh->stdin_copy = dup(STDIN_FILENO);
	while (cmd)
	{
		append_str(&cmd->path, "/");
		if (cmd->next != NULL)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)
		{
			if (child_process_pipeline(pipefd, cmd, envp, sh) == -1)
				return ;
			// exit code in syscall error?
		}
		else
		{
			wait(NULL); // maybe wait outside of loop to handle last exit code
			if (cmd->next != NULL)
				close(pipefd[1]);
			cmd = cmd->next;
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
		}
	}
	dup2(sh->stdin_copy, STDIN_FILENO);
	close(sh->stdin_copy);
	free_arr(envp);
}

int	child_process_single_cmd(t_cmd *cmd, char **envp)
{
	char	*cmd_path;

	if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	// handle Signals
	cmd_path = get_cmd_path(cmd);
	// do for commands without forward slash first, then later handle relative and absolute paths
	if (execve(cmd_path, cmd->opt, envp) == -1)
		free(cmd_path);
	return (-1);
}

void	exec_as_child(t_cmd *cmd, t_shell *sh)
{
	char	**envp;
	pid_t	pid;

	envp = get_env_arr(sh->env);
	append_str(&cmd->path, "/");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (child_process_single_cmd(cmd, envp) == -1)
			return ;
		// exit code on sys call error?
	}
	else
	{
		wait(NULL); // maybe wait outside of loop to handle last exit code
	}
	free_arr(envp);
}

/*
	if (fd_in != -1 && close(fd_in) < 0)
		ft_error(strerror(errno), 0);
	if (close(fd_out) < 0)
		return (ft_error(strerror(errno), 0));
*/