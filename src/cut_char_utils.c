/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_char_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:44:19 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 15:24:50 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_str_env(char *str, t_env *env, int k)
{
	t_env_node	*tmp;
	char		*new;
	char		*nb;
	int			size;

	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	if (!ft_strncmp(new, "?", 2))
	{
		new = free_null(new);
		nb = ft_itoa(g_exit_code);
		size = (int)ft_strlen(nb);
		nb = free_null(nb);
		return (size);
	}
	tmp = find_env_node(env, new);
	new = free_null(new);
	if (tmp == NULL)
		return (0);
	else
		return ((int)ft_strlen(tmp->value));
}

char	*get_str_env(char *str, t_env *env, int k)
{
	t_env_node	*tmp;
	char		*new;

	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	if (!ft_strncmp(new, "?", 2))
	{
		new = free_null(new);
		return (ft_itoa(g_exit_code));
	}
	tmp = find_env_node(env, new);
	new = free_null(new);
	if (tmp == NULL)
		return (0);
	else
		return (tmp->value);
}

void	help_count_dq(char *str, int *i, t_env *env)
{
	i[4] += 2;
	i[1] = get_next_char(&str[i[0] + 1], "$") + 1;
	while (i[1] < get_end_quote(&str[i[0] + 1], '"'))
	{
		i[2] = get_len_var(&str[i[1] + 1]);
		if (i[2] != 0)
		{
			i[3] = check_str_env(&str[i[0] + i[1] + 1], env, i[2]);
			i[4] += i[2] + 1 - i[3];
		}
		i[1] += get_next_char(&str[i[0] + i[1] + i[2] + 1], "$") + i[2] + 1;
	}
	i[0] += get_end_quote(&str[i[0] + 1], '"') + 2;
}

void	help_count_dollar(char *str, int *i, t_env *env)
{
	i[2] = get_len_var(&str[i[0] + 1]);
	if (i[2] != 0)
	{
		i[3] = check_str_env(&str[i[0] + 1], env, i[2]);
		i[4] += i[2] + 1 - i[3];
	}
	i[0] += i[2] + 1;
}

int	count_chars(char *str, t_env *env)
{
	int	i[5];

	init_idx(i, 5);
	while (str[i[0]] && i[0] < (int)ft_strlen(str))
	{
		if (str[i[0]] == '"')
			help_count_dq(str, i, env);
		else if (str[i[0]] == '\'')
		{
			i[4] += 2;
			i[0] += get_end_quote(&str[i[0] + 1], '\'') + 2;
		}
		else if (str[i[0]] == '$')
			help_count_dollar(str, i, env);
		else
			i[0]++;
	}
	return (i[4]);
}
