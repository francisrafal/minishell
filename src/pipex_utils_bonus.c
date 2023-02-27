

int	process_one(int *pipefd, t_cmd *c1, char **envp)
{
	int		i;
	char	*cmd;

	i = 0;
	if (c1->fd == -1)
		return (-1);
	if (dup2(c1->fd, STDIN_FILENO) < 0 || dup2(pipefd[1], STDOUT_FILENO) < 0)
	{
		perror("Process one");
		return (-1);
	}
	close(pipefd[0]);
	while (c1->path[i])
	{
		cmd = ft_strjoin_path(c1->path[i], c1->opt[0]);
		if (!cmd)
			return (-1);
		if (execve(cmd, c1->opt, envp) == -1)
			free(cmd);
		i++;
	}
	return (-1);
}

int	process_two(int *pipefd, t_cmd *c2, int fd, char **envp)
{
	int		i;
	char	*cmd;

	i = 0;
	if (dup2(fd, STDOUT_FILENO) < 0 || dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("Process two");
		return (-1);
	}
	close(pipefd[1]);
	while (c2->path[i])
	{
		cmd = ft_strjoin_path(c2->path[i], c2->opt[0]);
		if (!cmd)
			return (-1);
		if (execve(cmd, c2->opt, envp) == -1)
			free(cmd);
		i++;
	}
	return (-1);
}

void	pipex(int fd_in, int fd_out, t_cmd **lst, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	t_cmd	*cmd;

	cmd = *lst;
	cmd->fd = fd_in;
	while (cmd->next)
	{
		pipe_help(pipefd, &pid);
		if (pid == 0)
		{
			if (process_one(pipefd, cmd, envp) == -1)
				return ;
		}
		else
		{
			wait(NULL);
			close(pipefd[1]);
			cmd = cmd->next;
			cmd->fd = pipefd[0];
		}
	}
	process_two(pipefd, cmd, fd_out, envp);
	free_lst(lst);
}

/*
	if (fd_in != -1 && close(fd_in) < 0)
		ft_error(strerror(errno), 0);
	if (close(fd_out) < 0)
		return (ft_error(strerror(errno), 0));
*/