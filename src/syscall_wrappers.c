/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrappers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:49:32 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:43:53 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_or_print_error(int fd)
{
	if (fd == -1)
		return ;
	if (close(fd) == -1)
		perror("close");
}

int	dup2_or_print_error(int oldfd, int newfd)
{
	int	fd;

	fd = dup2(oldfd, newfd);
	if (fd == -1)
		perror("dup2");
	return (fd);
}

void	unlink_heredoc(t_cmd *cmd)
{
	char	*name;

	if (!cmd->read_in)
		return ;
	name = get_heredoc_name(cmd->cmd_id);
	if (unlink(name) == -1)
		perror("unlink");
	name = free_null(name);
}

pid_t	fork_or_print_error(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return (pid);
}

int	pipe_or_print_error(int pipefd[2])
{
	int	ret;

	ret = pipe(pipefd);
	if (pipe(pipefd) == -1)
		perror("pipe");
	return (ret);
}
