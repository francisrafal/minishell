#include "minishell.h"

static int	check_char(char *str, char c, int *j)
{
	int	i;

	i = 1;
	if (str[i] && str[i] == c)
		i++;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		else if (str[i] != '\t' && str[i] != ' ')
		{
			*j += i;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	check_line(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			i += get_end_quote(&str[i + 1], '"') + 1;
		else if (str[i] == '\'')
			i += get_end_quote(&str[i + 1], '\'') + 1;
		else if (str[i] == '>')
		{
			if (check_char(&str[i], '>', &i))
				return (1);
		}
		else if (str[i] == '<')
		{
			if (check_char(&str[i], '<', &i))
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_syntax(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (check_line(cmds[i]))
			return (1);
		i++;
	}
	return (0);
}
