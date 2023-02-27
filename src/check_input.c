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

t_pipe_state	pipe_transition(t_pipe_state state, char c)
{
	if (state == START)
	{
		if (c == ' ' || c == '\t')
			return (START);
		if (c != '|')
			return (CMD);
	}
	if (state == CMD)
	{
		if (c != '|')
			return (CMD);
		if (c == '|')
			return (PIPE);
	}
	if (state == PIPE)
	{
		if (c == ' ' || c == '\t')
			return (BLANK);
		if (c != '|')
			return (CMD);
		if (c == '|')
			return (MULTIPLE_PIPES);
	}
	if (state == BLANK)
	{
		if (c == ' ' || c == '\t')
			return (BLANK);
		if (c != '|')
			return (CMD);
		if (c == '|')
			return (MULTIPLE_PIPES);
	}
	if (state == BEGIN_PIPE)
		return (BEGIN_PIPE);
	if (state == MULTIPLE_PIPES)
		return (MULTIPLE_PIPES);
	return (REJECT);
}

int	check_pipes(char *str)
{
	t_pipe_state	state;
	int				i;

	i = 0;
	state = START;
	while (str[i])
		state = pipe_transition(state, str[i++]);
	if (state == START || state == CMD)
		return (0);
	ft_error("", "syntax error near unexpected token `|'");
	return (1);
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
