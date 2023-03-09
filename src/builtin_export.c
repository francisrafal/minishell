/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 16:48:08 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:48:17 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_export_no_args(char **cmd_args, t_shell *sh, int mode)
{
	t_env		*tmp;
	t_env_node	*runner;	

	(void)mode;
	(void)cmd_args;
	tmp = env_dup(sh->env);
	sort_env(tmp);
	runner = tmp->head;
	if (runner == NULL)
		return (0);
	while (runner)
	{
		if (ft_strncmp(runner->key, "_", 2) == 0)
		{
			runner = runner->next;
			continue ;
		}
		if (runner->value == NULL)
			printf("declare -x %s\n", runner->key);
		else
			print_with_escaped_quotes(runner->key, runner->value);
		runner = runner->next;
	}	
	tmp = free_env_null(tmp);
	return (0);
}

static void	help_export_with_args(char *cmd_arg, t_shell *sh)
{
	t_env_node	*new_node;
	t_env_node	*node;

	new_node = create_env_node(cmd_arg);
	node = find_env_node(sh->env, new_node->key);
	if (node != NULL)
	{
		replace_node_value(node, new_node->value);
		new_node = free_env_node_null(new_node);
	}
	else
		append_env(sh->env, new_node);
}

int	bi_export_with_args(char **cmd_args, t_shell *sh, int mode, int argc)
{
	int			i;
	int			error;

	(void)mode;
	error = 0;
	i = 1;
	while (i < argc)
	{
		if (ft_isalpha(cmd_args[i][0]) || (cmd_args[i][0] == '_'))
			help_export_with_args(cmd_args[i], sh);
		else
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd_args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			error = 1;
		}
		i++;
	}
	if (error)
		return (1);
	return (0);
}

int	bi_export(char **cmd_args, t_shell *sh, int mode)
{
	int	argc;

	(void)mode;
	argc = get_arr_size(cmd_args);
	if (argc == 1)
		return (bi_export_no_args(cmd_args, sh, mode));
	else
		return (bi_export_with_args(cmd_args, sh, mode, argc));
}
