/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:49:41 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:43:58 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	*wait_for_children(t_shell *sh, int children)
{
	int	i;

	i = 0;
	while (i < children)
	{
		if (waitpid(sh->pid[i], &sh->wstatus, 0) == -1)
			perror("waitpid");
		i++;
	}
	if (WIFEXITED(sh->wstatus))
	{
		if (WEXITSTATUS(sh->wstatus) != EXIT_NO_ARG)
			g_exit_code = WEXITSTATUS(sh->wstatus);
	}
	sh->pid = free_null(sh->pid);
	return (sh->pid);
}
