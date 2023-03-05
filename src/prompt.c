/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:48:33 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:48:35 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_line(void)
{
	char	*cmd_line;

	cmd_line = readline(PROMPT);
	if (cmd_line == NULL)
		return (ft_strdup("exit"));
	if (cmd_line[0] == 0)
		return (free_null(cmd_line));
	return (cmd_line);
}
