#include "minishell.h"

void	prun_help1(char *str, char *new, int *i);
void	prun_help2(char *str, char *new, int *i);

void	help_count(char *str, int *idx, int *count)
{
	int	i;
	int	j;

	i = *idx;
	j = 0;
	if (str[i] && str[i] == '"' && str[i + 1])
	{
		j = get_end_quote(&str[i + 1], '"') + 1;
		*count += j;
		i += j;
	}
	else if (str[i] && str[i] == '\'' && str[i + 1])
	{
		j = get_end_quote(&str[i + 1], '\'') + 1;
		*count += j;
		i += j;
	}
	while (str[i] && !(str[i] == ' ' || str[i] == '\t'))
	{
		*count += 1;
		i++;
	}
	*idx = i;
}

int	count_prun(char *str)
{
	int	i;
	int	count;
	int	flg;

	flg = 0;
	i = 0;
	count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		{
			flg = 1;
			i++;
		}
		if (flg && str[i])
			count++;
		flg = 0;
		help_count(str, &i, &count);
	}
	return (count);
}

char	*prun_str(char *str)
{
	int		i[4];
	char	*new;

	init_idx(i, 4);
	i[2] = count_prun(str);
	new = (char *)malloc(sizeof(char) * (i[2] + 1));
	if (!new)
		return (NULL);
	while (str[i[0]] == ' ' || str[i[0]] == '\t')
		i[0] += 1;
	while (str[i[0]])
	{
		prun_help1(str, new, i);
		prun_help2(str, new, i);
	}
	new[i[1]] = '\0';
	free_null(str);
	return (new);
}

void	prun_help1(char *str, char *new, int *i)
{
	while (str[i[0]] && (str[i[0]] == ' ' || str[i[0]] == '\t'))
	{
		i[3] = 1;
		i[0] += 1;
	}
	if (i[3] && str[i[0]])
	{
		new[i[1]] = ' ';
		i[1] += 1;
	}
	i[3] = 0;
}

void	prun_help2(char *str, char *new, int *i)
{
	int	j;

	j = 0;
	if (str[i[0]] && str[i[0]] == '"' && str[i[0] + 1])
	{
		j = get_end_quote(&str[i[0] + 1], '"') + i[0];
		while (i[0] <= j)
		{
			new[i[1]] = str[i[0]];
			i[1] += 1;
			i[0] += 1;
		}
	}
	else if (str[i[0]] && str[i[0]] == '\'' && str[i[0] + 1])
	{
		j = get_end_quote(&str[i[0] + 1], '\'') + i[0];
		while (i[0] <= j)
		{
			new[i[1]] = str[i[0]];
			i[1] += 1;
			i[0] += 1;
		}
	}
	while (str[i[0]] && !(str[i[0]] == ' ' || str[i[0]] == '\t'))
	{
		new[i[1]] = str[i[0]];
		i[1] += 1;
		i[0]++;
	}
}
