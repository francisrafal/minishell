/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:10:48 by frafal            #+#    #+#             */
/*   Updated: 2023/03/05 14:17:30 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_echo_options(char **cmd_args, int i, int *new_line)
{
	while (cmd_args[i][0] == '-')
	{
		if (cmd_args[i][1] == 'n')
		{
			*new_line = 0;
			i++;
		}
		else
			break ;
		if (cmd_args[i] == NULL)
			return (-1);
	}
	return (i);
}

int	bi_echo(char **cmd_args)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	if (cmd_args[i] == NULL)
	{
		printf("\n");
		return (0);
	}	
	i = parse_echo_options(cmd_args, i, &new_line);
	if (i == -1)
		return (0);
	while (cmd_args[i])
	{
		printf("%s", cmd_args[i]);
		if (cmd_args[i + 1])
			printf(" ");
		i++;
	}	
	if (new_line)
		printf("\n");
	return (0);
}
