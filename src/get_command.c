/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:47:20 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 15:42:46 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_empty_opt(char **cmds, char **tmp2)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmds[i])
	{
		if (!ft_strncmp(cmds[i], "", 1) && ft_strncmp(tmp2[i], "\"\"", 3))
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

char	**cut_empty_opt(char **cmds, char **org, int nopt)
{
	int		size;
	char	**tmp;
	int		i[2];

	init_idx(i, 2);
	size = nopt - count_empty_opt(cmds, org);
	if (count_empty_opt(cmds, org) == 0)
		return (cmds);
	if (size == 0)
		return (only_empty_opt(cmds));
	tmp = (char **)malloc(sizeof(char *) * (size + 1));
	while (i[0] < size)
	{
		if (!ft_strncmp(cmds[i[1]], "", 1) && ft_strncmp(org[i[1]], "\"\"", 3))
			i[1]++;
		tmp[i[0]] = ft_strdup(cmds[i[1]]);
		i[0]++;
		i[1]++;
	}
	tmp[i[0]] = NULL;
	cmds = free_arr_null(cmds);
	return (tmp);
}

char	*get_command(t_cmd *cmd, char *cmd_str, t_env *env)
{
	int		nopt;
	int		i;
	char	*str;
	char	**tmp;

	str = prun_str(cmd_str);
	cmd->opt = split_char(str, &nopt, ' ');
	tmp = split_char(str, &nopt, ' ');
	if (!cmd->opt || !tmp)
		return (NULL);
	i = 0;
	while (cmd->opt[i])
	{
		cmd->opt[i] = replace_chars(cmd->opt[i], env);
		if (!cmd->opt[i])
			return (NULL);
		i++;
	}
	cmd->opt = cut_empty_opt(cmd->opt, tmp, nopt);
	free_arr_null(tmp);
	return (str);
}
