/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:45:49 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:45:54 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_exit(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

int	ft_error(char *str, char *err)
{
	int	i;

	if (!str)
		return (2);
	if (err)
	{
		write(2, "minishell: ", 11);
		write(2, err, ft_strlen((const char *)err));
		if (str[0] != '\0')
			write(2, ": ", 2);
	}
	i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
	write(2, "\n", 1);
	return (2);
}

void	print_exec_error(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->opt[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
