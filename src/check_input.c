/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 16:49:07 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:49:15 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_char(char *str, char c, int *j)
{
	int	i;

	i = 1;
	if (str[i] && (str[i] == c || str[i] == '\t' || str[i] == ' '))
		i++;
	while (str[i])
	{
		if (str[i] == c || str[i] == '<' || str[i] == '>')
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

int	check_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{	
		if (str[i] == '"' || str[i] == '\'')
		{
			if (i > 0 && str[i - 1] == '\\')
			{
				i++;
				continue ;
			}
			if (str[i] == '"')
				i += get_end_quote(&str[i + 1], '"') + 1;
			else
				i += get_end_quote(&str[i + 1], '\'') + 1;
			if (str[i] == '\0')
			{
				ft_error("", "syntax error with quotes");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	help_check_line(char *str, int *i)
{
	if (str[*i] == '>')
	{
		if (check_char(&str[*i], '>', i))
		{
			ft_error("", "syntax error with output redirection");
			return (1);
		}
	}
	else if (str[*i] == '<')
	{
		if (check_char(&str[*i], '<', i))
		{
			ft_error("", "syntax error with input redirection");
			return (1);
		}
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
		else if (str[i] == '>' || str[i] == '<')
		{	
			if (help_check_line(str, &i))
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
