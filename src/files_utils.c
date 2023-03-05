/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:46:55 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:46:58 by celgert          ###   ########.fr       */
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

char	*cut_word(char *str, char c)
{
	int		i[4];
	char	*new;
	char	*trim;

	init_idx(i, 4);
	i[2] = get_char(str, c);
	i[0] = i[2] + 1;
	if (str[i[0]] && str[i[0]] == c)
		i[0]++;
	while (str[i[0]] && (str[i[0]] == '\t' || str[i[0]] == ' '))
		i[0]++;
	i[1] = get_next_char(&str[i[0]], "\t ><");
	i[3] = (int)ft_strlen(str) - i[0] - i[1] + i[2];
	new = (char *)malloc(sizeof(char) * (i[3] + 1));
	if (!new)
		return (NULL);
	ft_strlcpy(new, str, i[2] + 1);
	ft_strlcpy(&new[i[2]], &str[i[0] + i[1]], i[3] - i[2] + 1);
	str = free_null(str);
	trim = ft_strtrim(new, "\t ");
	if (!trim)
		return (NULL);
	new = free_null(new);
	return (trim);
}

char	*get_file_name(char *str, char c)
{
	int		i;
	int		j;
	int		k;
	char	*file;
	char	*trim;

	k = get_char(str, c);
	i = k + 1;
	if (str[i] && str[i] == c)
		i++;
	while (str[i] && (str[i] == '\t' || str[i] == ' '))
		i++;
	j = get_next_char(&str[i], "\t ><");
	file = (char *)malloc(sizeof(char) * (j + 1));
	if (!file)
		return (NULL);
	ft_strlcpy(file, &str[i], j + 1);
	trim = ft_strtrim(file, "\"\'");
	if (!trim)
		return (NULL);
	file = free_null(file);
	return (trim);
}
