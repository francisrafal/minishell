/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:46:55 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:42:56 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_charoq(char *line, char *cset)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
			i += get_end_quote(&line[i + 1], '"') + 2;
		else if (line[i] == '\'')
			i += get_end_quote(&line[i + 1], '\'') + 2;
		else if (ft_strchr(cset, line[i]))
		{
			if (i > 0 && line[i - 1] == '\\')
			{
				i++;
				continue ;
			}
			return (i);
		}
		else
			i++;
	}
	return (i);
}

void	help_getstrcoq(char *line, int *i, char c)
{
	if (line[*i] == '"')
		*i += get_end_quote(&line[*i + 1], '"') + 2;
	else if (line[*i] == '\'')
		*i += get_end_quote(&line[*i + 1], '\'') + 2;
	else if (line[*i] != c)
		*i += 1;
}

char	*get_strchroq(char *line, char c)
{
	int	i;

	if (line == NULL)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == c)
		{
			if (i > 0 && line[i - 1] == '\\')
			{
				i++;
				continue ;
			}
			return (&line[i]);
		}
		help_getstrcoq(line, &i, c);
	}
	if ((char)c == 0)
		return (&line[i]);
	if (!line[i])
		return (NULL);
	return (&line[i]);
}

char	*cut_word(char *str, char c)
{
	int		i[4];
	char	*new;
	char	*trim;

	init_idx(i, 4);
	i[2] = get_charoq(str, &c);
	i[0] = i[2] + 1;
	if (str[i[0]] && str[i[0]] == c)
		i[0]++;
	while (str[i[0]] && (str[i[0]] == '\t' || str[i[0]] == ' '))
		i[0]++;
	i[1] = get_charoq(&str[i[0]], "\t ><");
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

	k = get_charoq(str, &c);
	i = k + 1;
	if (str[i] && str[i] == c)
		i++;
	while (str[i] && (str[i] == '\t' || str[i] == ' '))
		i++;
	j = get_charoq(&str[i], "\t ><");
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
