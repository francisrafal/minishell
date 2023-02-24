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
int	check_pipes(char *str)
{
	int i;
	int flg;
	i = 0;
	flg = 0;
	while (str[i] && str[i] != '|')
	{
			flg = 1;
			i++;
	}
	while (str[i])
	{
		if (str[i] == '|' && flg == 0)
		 	break ;
		flg = 0;
		while (str[i] && str[i] != '|')
		{
			flg = 1;
			i++;
		}
	}
	if (flg == 0)
	{ 
			ft_error("", "syntax error near unexpected '|'");
			return (1);
	}
	return (0);
}

int	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{	
		if (str[i] == '"' || str[i] == '\'')
		{
			if (i>0 && str[i-1] == '\\')
			{
				i++;
				continue;
			}
			if (str[i] == '"')
				i += get_end_quote(&str[i + 1], '"') + 1;
			else
				i += get_end_quote(&str[i + 1], '\'') + 1;
			if (str[i] == '\0')
			{
				ft_error("", "syntax error with qoutes");
				return (1);
			}
		}
                i++;
	}
	return (0);
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
			{
				ft_error("", "syntax error with input redirection");
				return (1);
			}
		}
		else if (str[i] == '<')
		{
			if (check_char(&str[i], '<', &i))
			{
				ft_error("syntax error with output redirection","");
				return (1);
			}
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
