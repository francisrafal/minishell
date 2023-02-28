#include "minishell.h"

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
