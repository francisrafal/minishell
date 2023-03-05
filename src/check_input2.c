/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:43:11 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:43:16 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pipe_state	dfa_pipe_transition2(t_pipe_state state, char c)
{
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
	return (state);
}

static t_pipe_state	dfa_pipe_transition(t_pipe_state state, char c)
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
	if (state == PIPE || state == BLANK)
		return (dfa_pipe_transition2(state, c));
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
		state = dfa_pipe_transition(state, str[i++]);
	if (state == START || state == CMD)
		return (0);
	ft_error("", "syntax error near unexpected token `|'");
	return (1);
}
