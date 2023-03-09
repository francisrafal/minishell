/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:46:55 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 10:44:44 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_char(char *line, char *cset)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_strchr(cset, line[i]))
		{
			if (i > 0 && line[i - 1] == '\\'
				&& line[i] != '\t' && line[i] != ' ')
			{
				i++;
				continue ;
			}
			return (i);
		}
		i++;
	}
	return (i);
}

int	get_char(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == c)
		{
			if (i > 0 && line[i - 1] == '\\'
				&& line[i] != '\t' && line[i] != ' ')
			{
				i++;
				continue ;
			}
			return (i);
		}
		i++;
	}
	return (i);
}

int	get_len_var(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	return (i);
}
