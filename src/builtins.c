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

int	bi_exit(t_cmd *cmd, t_shell *sh, int mode)
{
	int	argc;
	int	code;

	argc = get_arr_size(cmd->opt);
	if (mode == EXEC_AS_PARENT)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
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
		if (argc > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1);
		}
	}
	if (mode == EXEC_AS_CHILD)
	{
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
		if (argc > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			cmd = free_lst_null(cmd);
			sh = free_shell_null(sh);
			exit(EXIT_FAILURE);
		}
	}
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

int	bi_export_with_args(char **cmd_args, t_shell *sh, int mode, int argc)
{
	int			i;
	int			error;
	t_env_node	*new_node;
	t_env_node	*node;

	(void)mode;
	error = 0;
	i = 1;
	while (i < argc)
	{
		if (ft_isalpha(cmd_args[i][0]) || (cmd_args[i][0] == '_'))
		{
			new_node = create_env_node(cmd_args[i]);
			node = find_env_node(sh->env, new_node->key);
			if (node != NULL)
			{
				replace_node_value(node, new_node->value);
				new_node = free_env_node_null(new_node);
			}
			else
				append_env(sh->env, new_node);
		}
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
