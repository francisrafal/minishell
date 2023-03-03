#include "minishell.h"

int	get_end_quote(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == c)
		{
			if (i == 0)
				return (i);
			else if (i > 0 && line[i - 1] != '\\')
				return (i);
		}
		i++;
	}
	return (i);
}

int	count_cmds(char *line, char c)
{
	int	i;
	int	ncmds;

	ncmds = 1;
	i = 0;
	while (line[i] && i < (int)ft_strlen(line))
	{
		if (line[i] == '"' && line[i + 1])
		{
			i += get_end_quote(&line[i + 1], '"') + 1;
		}
		else if (line[i] == '\'' && line[i + 1])
			i += get_end_quote(&line[i + 1], '\'') + 1;
		else if (line[i] == c)
			ncmds += 1;
		else if (line[i] == '#')
			return (ncmds);
		if (line[i] && i != (int)ft_strlen(line))
			i++;
	}
	return (ncmds);
}

char	*fill_cmds(char *line, int len)
{
	char	*str;
	char	*trim;

	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, line, len + 1);
	trim = ft_strtrim(str, "\t ");
	str = free_null(str);
	return (trim);
}

static void	split_char_help(char *line, char **cmds, int *idx, char c)
{
	if (line[idx[0]] == '"')
		idx[0] += get_end_quote(&line[idx[0] + 1], '"') + 1;
	else if (line[idx[0]] == '\'')
		idx[0] += get_end_quote(&line[idx[0] + 1], '\'') + 1;
	else if (line[idx[0]] == c)
	{
		cmds[idx[2]] = fill_cmds(&line[idx[1]], idx[0] - idx[1]);
		idx[2] += 1;
		idx[1] = idx[0] + 1;
	}
}

char	**split_char(char *line, int *ncmds, char c)
{
	char	**cmds;
	int		len;
	int		idx[3];

	if (!line)
		return (NULL);
	len = count_cmds(line, c);
	*ncmds = len;
	init_idx(idx, 3);
	cmds = (char **)malloc(sizeof(char *) * (len + 1));
	if (!cmds)
		return (NULL);
	while (line[idx[0]] && idx[2] < len)
	{
		if (line[idx[0]] == '#')
			break ;
		else
			split_char_help(line, cmds, idx, c);
		idx[0] += 1;
	}
	cmds[idx[2]] = fill_cmds(&line[idx[1]], idx[0] - idx[1]);
	cmds[idx[2] +1] = NULL;
	return (cmds);
}
