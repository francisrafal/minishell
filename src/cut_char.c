#include "minishell.h"

void	help_replace_dq2(char *str, char *line, int *i, t_env *env)
{
	char	*tmp;

	i[3] = check_str_env(&str[i[0] + 1], env, i[2]);
	if (i[3])
	{	
		tmp = get_str_env(&str[i[0] + 1], env, i[2]);
		ft_strlcpy(&line[i[4]], tmp, i[3] + 1);
		if (str[i[0] + 1] == '?' && i[3] == 1)
			tmp = free_null(tmp);
		i[4] += i[3];
		i[0] += i[2];
	}
	else
		i[0] += i[2];
}

void	help_replace_dq(char *str, char *line, int *i, t_env *env)
{
	int	k;

	i[1] = get_next_char(&str[i[0] + 1], "$") + 1;
	while (i[1] < get_end_quote(&str[i[0] + 1], '"'))
	{
		ft_strlcpy(&line[i[4]], &str[i[0] + 1], i[1]);
		i[0] += i[1];
		i[4] += i[1] - 1;
		i[2] = get_next_char(&str[i[0] + 1], "\t \"\'");
		if (i[2] != 0)
			help_replace_dq2(str, line, i, env);
		i[1] += get_next_char(&str[i[0] + 1], "$");
	}
	k = get_end_quote(&str[i[0] + 1], '"');
	ft_strlcpy(&line[i[4]], &str[i[0] + 1], k + 1);
	i[4] += k;
	i[0] += k + 2;
}

void	help_replace_dollar(char *str, char *line, int *i, t_env *env)
{
	char	*tmp;

	i[2] = get_next_char(&str[i[0] + 1], "\t \"\'\0");
	if (i[2] != 0)
	{
		i[3] = check_str_env(&str[i[0] + 1], env, i[2]);
		if (i[3])
		{
			tmp = get_str_env(&str[i[0] + 1], env, i[2]);
			ft_strlcpy(&line[i[4]], tmp, i[3] + 1);
			if (str[i[0] + 1] == '?' && i[3] > 0)
				tmp = free_null(tmp);
			i[4] += i[3];
			i[0] += i[2] + 1;
		}
		else
			i[0] += i[2] + 1;
	}
}

void	help_replace(char *str, char *line, int *i, t_env *env)
{
	if (str[i[0]] == '\'')
	{
		i[3] = get_end_quote(&str[i[0] + 1], '\'');
		ft_strlcpy(&line[i[4]], &str[i[0] + 1], i[3] + 1);
		i[4] += i[3];
		i[0] += i[3] + 2;
	}
	else if (str[i[0]] == '$')
		help_replace_dollar(str, line, i, env);
	else
	{
		ft_strlcpy(&line[i[4]], &str[i[0]], 2);
		i[0]++;
		i[4]++;
	}
}

char	*replace_chars(char *str, t_env *env)
{
	char	*line;
	int		i[6];

	init_idx(i, 6);
	i[5] = (int)ft_strlen(str) - count_chars(str, env);
	line = (char *)malloc(sizeof(char) * (i[5] + 1));
	if (!line)
		return (NULL);
	if (i[5] == 0)
		line[0] = '\0';
	while (str[i[0]] && i[0] < (int)ft_strlen(str))
	{
		if (str[i[0]] == '"')
			help_replace_dq(str, line, i, env);
		else
			help_replace(str, line, i, env);
	}
	str = free_null(str);
	return (line);
}
