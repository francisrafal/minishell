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
	while ((cmd->path)[i] != NULL)
	{
		cmd_path = ft_strjoin(cmd->path[i], cmd->opt[0]);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		cmd_path = free_null(cmd_path);
		i++;
	}

	//perror_exit("access");
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

	if (cmd->fd_in == -1)
		return (-1);
	if (cmd->next != NULL)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (-1);
		}
	}
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
	close(pipefd[0]);
	close(sh->stdin_copy);
	if (is_builtin(cmd))
	{
		g_exit_code = exec_builtin(cmd, sh, EXEC_AS_CHILD);
		cmd = free_lst_null(cmd);
		sh = free_data_null(sh);
		envp = free_arr_null(envp);
		exit(g_exit_code);
	}
	else
	{
		if (cmd->opt[0][0] == '\0')
		{
			cmd = free_lst_null(cmd);
			sh = free_data_null(sh);
			envp = free_arr_null(envp);
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
		sh = free_data_null(sh);
		if (execve(cmd_path, cmd->opt, envp) == -1)
			cmd_path = free_null(cmd_path);
	}
	return (-1);
}

void	exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	int		pipefd[2];
	char	**envp;
	int		i;
	int		j;

	sh->pid = malloc(sizeof (pid_t) * cmd->ncmds);
	if (sh->pid == NULL)
		return ;
	envp = get_env_arr(sh->env);
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
				return ;
			}
		}
		sh->pid[i] = fork();
		if (sh->pid[i] == -1)
		{
			perror("fork");
			return ;
		}
		if (sh->pid[i] == 0)
		{
			if (child_process_pipeline(pipefd, cmd, envp, sh) == -1)
			{
				envp = free_arr_null(envp);
				print_exec_error(cmd);
				if (errno == 13)
					exit(126);
				if (errno == 2)
					exit(127);
				exit(127);
			}
		}
		else
		{
			if (cmd->next != NULL)
				close(pipefd[1]);
			cmd = cmd->next;
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			i++;
		}
	}
	j = 0;
	while (j < i)
	{
		waitpid(sh->pid[j], &sh->wstatus, 0);
		j++;
	}
	if (WIFEXITED(sh->wstatus))
	{
		if (WEXITSTATUS(sh->wstatus) != EXIT_NO_ARG)
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	dup2(sh->stdin_copy, STDIN_FILENO);
	close(sh->stdin_copy);
	envp = free_arr_null(envp);
	sh->pid = free_null(sh->pid);
}

int	child_process_single_cmd(t_cmd *cmd, char **envp, t_shell *sh)
{
	char	*cmd_path;

	if (cmd->fd_in == -1)
		return (-1);
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
	if (is_builtin(cmd))
	{
		g_exit_code = exec_builtin(cmd, sh, EXEC_AS_CHILD);
        cmd = free_lst_null(cmd);
		sh = free_data_null(sh);
		envp = free_arr_null(envp);
		exit(g_exit_code);
	}
	else
	{
		if (cmd->opt[0][0] == '\0')
		{
			printf("we are here\n");
			cmd = free_lst_null(cmd);
			sh = free_data_null(sh);
			envp = free_arr_null(envp);
			exit (0);
		}
		sh = free_data_null(sh);
		if (cmd->opt[0] - ft_strchr(cmd->opt[0], '/') == (long)cmd->opt[0])
		{
			cmd_path = get_cmd_path(cmd);
			if (cmd_path == NULL)
				return (-1);
		}
		else
			cmd_path = ft_strdup(cmd->opt[0]);
		if (execve(cmd_path, cmd->opt, envp) == -1)
			cmd_path = free_null(cmd_path);
	}
	return (-1);
}

void	exec_one_child(t_cmd *cmd, t_shell *sh)
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
		if (child_process_single_cmd(cmd, envp, sh) == -1)
		{
			envp = free_arr_null(envp);
			print_exec_error(cmd);
			if (errno == 13)
				exit(126);
			if (errno == 2)
				exit(127);
			exit(127);
		}
	}
	else
	{
		wait(&sh->wstatus);
		if (WIFEXITED(sh->wstatus))
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	envp = free_arr_null(envp);
}

/*
	if (fd_in != -1 && close(fd_in) < 0)
		ft_error(strerror(errno), 0);
	if (close(fd_out) < 0)
		return (ft_error(strerror(errno), 0));
*/
