/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 13:13:35 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 14:24:26 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	help_exit_parent(t_cmd *cmd, t_shell *sh, int argc)
{
	int	code;

	if (argc == 1)
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(EXIT_SUCCESS);
	}
	if (argc > 1 && !ft_isdigit(cmd->opt[1][0]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->opt[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(2);
	}
	if (argc == 2)
	{
		code = ft_atoi(cmd->opt[1]);
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(code);
	}
}

static void	help_exit_child(t_cmd *cmd, t_shell *sh, int argc)
{
	int	code;

	if (argc == 1)
	{
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(EXIT_NO_ARG);
	}
	if (argc > 1 && !ft_isdigit(cmd->opt[1][0]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->opt[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(2);
	}
	if (argc == 2)
	{
		code = ft_atoi(cmd->opt[1]);
		cmd = free_lst_null(cmd);
		sh = free_shell_null(sh);
		exit(code);
	}
}

static void	help_exit_child2(t_cmd *cmd, t_shell *sh)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	cmd = free_lst_null(cmd);
	sh = free_shell_null(sh);
	exit(EXIT_FAILURE);
}

int	bi_exit(t_cmd *cmd, t_shell *sh, int mode)
{
	int	argc;

	argc = get_arr_size(cmd->opt);
	if (mode == EXEC_AS_PARENT)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		if (argc == 1 || argc == 2 || (argc > 1 && !ft_isdigit(cmd->opt[1][0])))
			help_exit_parent(cmd, sh, argc);
		if (argc > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1);
		}
	}
	if (mode == EXEC_AS_CHILD)
	{
		if (argc == 1 || argc == 2 || (argc > 1 && !ft_isdigit(cmd->opt[1][0])))
			help_exit_child(cmd, sh, argc);
		if (argc > 2)
			help_exit_child2(cmd, sh);
	}
	return (0);
}
