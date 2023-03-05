/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:42:06 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 11:42:27 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_echo(char **cmd_args, t_shell *sh, int mode)
{
	int	i;
	int	new_line;

	(void)sh;
	(void)mode;
	new_line = 1;
	i = 1;
	if (cmd_args[i] == NULL)
	{
		printf("\n");
		return (0);
	}	
	while (cmd_args[i][0] == '-')
	{
		if (cmd_args[i][1] == 'n')
		{
			new_line = 0;
			i++;
		}
		else
			break ;
		if (cmd_args[i] == NULL)
			return (0);
	}
	while (cmd_args[i])
	{
		printf("%s", cmd_args[i]);
		if (cmd_args[i + 1])
			printf(" ");
		i++;
	}	
	if (new_line)
		printf("\n");
	return (0);
}

int	bi_pwd(char **cmd_args, t_shell *sh, int mode)
{
	char	*cwd;

	(void)cmd_args;
	(void)sh;
	(void)mode;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	printf("%s\n", cwd);
	cwd = free_null(cwd);
	return (0);
}

int	bi_env(char **cmd_args, t_shell *sh, int mode)
{
	int			argc;
	t_env_node	*runner;	

	(void)mode;
	argc = get_arr_size(cmd_args);
	if (argc > 1)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
		cmd_args = free_arr_null(cmd_args);
		sh = free_shell_null(sh);
		exit(EXIT_FAILURE);
	}
	if (sh->env == NULL)
		return (0);
	runner = sh->env->head;
	if (runner == NULL)
		return (0);
	while (runner)
	{
		if (runner->value)
			printf("%s=%s\n", runner->key, runner->value);
		runner = runner->next;
	}	
	return (0);
}

int	bi_unset(char **cmd_args, t_shell *sh, int mode)
{
	int	argc;
	int	i;
	int	error;

	(void)mode;
	argc = get_arr_size(cmd_args);
	error = 0;
	i = 1;
	while (i < argc)
	{
		if (ft_isalpha(cmd_args[i][0]) || (cmd_args[i][0] == '_'))
			remove_env_node(sh->env, cmd_args[i]);
		else
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
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
