/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 13:13:47 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 13:13:50 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_cd(char **cmd_args, t_shell *sh, int mode)
{
	char		*cwd;
	char		*tmp;
	int			argc;
	t_env_node	*oldpwd;
	t_env_node	*pwd;

	(void)sh;
	(void)mode;
	if (cmd_args == NULL || cmd_args[1] == NULL)
		return (0);
	argc = get_arr_size(cmd_args);
	if (argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (chdir(cmd_args[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	oldpwd = find_env_node(sh->env, "OLDPWD");
	if (oldpwd == NULL)
		oldpwd = append_env(sh->env, create_env_node("OLDPWD="));
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	pwd = find_env_node(sh->env, "PWD");
	if (pwd == NULL)
	{
		tmp = ft_strjoin("PWD=", cwd);
		pwd = append_env(sh->env, create_env_node(tmp));
		tmp = free_null(tmp);
	}
	replace_node_value(oldpwd, pwd->value);
	replace_node_value(pwd, cwd);
	cwd = free_null(cwd);
	return (0);
}
