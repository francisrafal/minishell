/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 16:46:46 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:46:51 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_arr_size(char **arr)
{
	int	argc;

	argc = 0;
	while (arr[argc] != NULL)
		argc++;
	return (argc);
}

void	print_with_escaped_quotes(char *key, char *value)
{
	int	i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(key, STDOUT_FILENO);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	i = 0;
	while (value[i])
	{
		if (value[i] == '"')
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(value[i], STDOUT_FILENO);
		i++;
	}
	ft_putstr_fd("\"\n", STDOUT_FILENO);
}
