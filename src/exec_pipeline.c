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
	{
		cmd = free_lst_null(cmd);
		sh = free_data_null(sh);
		envp = free_arr_null(envp);
		exit (1);
	}	
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: after fork", getpid());
	if (cmd->next != NULL)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (-1);
		}
		sleep(2);
		lsof("child: dup2(pipefd[1], STDOUT_FILENO)", getpid());
		close(pipefd[1]);
		sleep(2);
		lsof("child: close(pipefd[1])", getpid());
	}
	if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: dup2(cmd->fd_in, STDIN_FILENO)", getpid());
	if (cmd->re_in)
		close(cmd->fd_in);
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: close(cmd->fd_in)", getpid());
	if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: dup2(cmd->fd_out, STDOUT_FILENO)", getpid());
	if (cmd->re_out)
		close(cmd->fd_out);
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: close(cmd->fd_out)", getpid());
	close(pipefd[0]);
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: close(pipefd[0])", getpid());
	close(sh->stdin_copy);
	if (cmd->next == NULL)
		sleep(3);
	else
		sleep(2);
	lsof("child: close(sh->stdin_copy)", getpid());
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
	ft_putstr_fd("Parent PID: ", STDERR_FILENO);
	ft_putnbr_fd(getpid(), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	lsof("Parent: before stdin_copy", getpid());
	sh->stdin_copy = dup(STDIN_FILENO);
	lsof("Parent: stdin_copy", getpid());
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
			lsof("Parent: pipe(pipefd)", getpid());
		}
		lsof("Parent: before fork", getpid());
		sh->pid[i] = fork();
		if (sh->pid[i] == -1)
		{
			perror("fork");
			return ;
		}
		if (sh->pid[i] == 0)
		{
			ft_putstr_fd("Child PID: ", STDERR_FILENO);
			ft_putnbr_fd(getpid(), STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
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
			lsof("parent: after fork", getpid());
			if (cmd->re_in)
			{
				close(cmd->fd_in);
				lsof("parent: close(cmd->fd_in)", getpid());
			}
			if (cmd->re_out)
			{
				close(cmd->fd_out);
				lsof("parent: close(cmd->fd_out)", getpid());
			}
			if (cmd->next != NULL)
			{
				close(pipefd[1]);
				lsof("parent: close(pipefd[1])", getpid());
				dup2(pipefd[0], STDIN_FILENO);
				lsof("parent: dup2(pipefd[0], STDIN_FILENO)", getpid());
			}
			close(pipefd[0]);
			lsof("parent: close(pipefd[0])", getpid());
			cmd = cmd->next;
			if (cmd == NULL)
			{
				dup2(sh->stdin_copy, STDIN_FILENO);
				lsof("parent: dup2(sh->stdin_copy, STDIN_FILENO)", getpid());
				close(sh->stdin_copy);
				lsof("parent: close(sh->stdin_copy)", getpid());
			}
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
	envp = free_arr_null(envp);
	sh->pid = free_null(sh->pid);
}

int	child_process_single_cmd(t_cmd *cmd, char **envp, t_shell *sh)
{
	char	*cmd_path;

	if (cmd->fd_in == -1)
	{
		cmd = free_lst_null(cmd);
		sh = free_data_null(sh);
		envp = free_arr_null(envp);
		exit (1);
	}
	sleep(1);
	lsof("child: after fork", getpid());
	if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (cmd->re_in)
		close(cmd->fd_in);
	sleep(1);
	lsof("child: dup2(cmd->fd_in, STDIN_FILENO)", getpid());
	if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	if (cmd->re_out)
		close(cmd->fd_out);
	sleep(1);
	lsof("child: dup2(cmd->fd_out, STDOUT_FILENO)", getpid());
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
	ft_putstr_fd("Parent PID: ", STDERR_FILENO);
	ft_putnbr_fd(getpid(), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	lsof("before fork", getpid());
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		ft_putstr_fd("Child PID: ", STDERR_FILENO);
		ft_putnbr_fd(getpid(), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
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
		if (cmd->re_in)
			close(cmd->fd_in);
		if (cmd->re_out)
			close(cmd->fd_out);
		lsof("parent: after fork", getpid());
		wait(&sh->wstatus);
		if (WIFEXITED(sh->wstatus))
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	envp = free_arr_null(envp);
}
