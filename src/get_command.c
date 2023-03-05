/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:47:20 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:47:22 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_empty_opt(char **cmds)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmds[i])
	{
		if (!ft_strncmp(cmds[i], "", 1))
			count++;
		i++;
	}
	return (count);
}

char	**only_empty_opt(char **cmds)
{
	char	**tmp;

	tmp = (char **)malloc(sizeof(char *) * 2);
	if (!tmp)
		return (NULL);
	tmp[0] = ft_strdup("");
	tmp[1] = NULL;
	cmds = free_arr_null(cmds);
	return (tmp);
}

char	**cut_empty_opt(char **cmds, int nopt)
{
	int		size;
	char	**tmp;
	int		i;
	int		j;

	size = nopt - count_empty_opt(cmds);
	if (count_empty_opt(cmds) == 0)
		return (cmds);
	if (size == 0)
		return (only_empty_opt(cmds));
	i = 0;
	j = 0;
	tmp = (char **)malloc(sizeof(char *) * (size + 1));
	while (i < size)
	{
		if (!ft_strncmp(cmds[j], "", 1))
			j++;
		tmp[i] = ft_strdup(cmds[j]);
		i++;
		j++;
	}
	tmp[i] = NULL;
	cmds = free_arr_null(cmds);
	return (tmp);
}

char	*get_command(t_cmd *cmd, char *cmd_str, t_env *env)
{
	int		nopt;
	int		i;
	char	*str;

	str = prun_str(cmd_str);
	cmd->opt = split_char(str, &nopt, ' ');
	if (!cmd->opt)
		return (NULL);
	i = 0;
	while (cmd->opt[i])
	{
		cmd->opt[i] = replace_chars(cmd->opt[i], env);
		if (!cmd->opt[i])
			return (NULL);
		i++;
	}
	cmd->opt = cut_empty_opt(cmd->opt, nopt);
	return (str);
}
