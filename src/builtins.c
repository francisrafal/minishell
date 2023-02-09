#include "minishell.h"

int	bi_echo(char **cmd_args, t_shell *sh, int mode)
{
	(void)sh;
	(void)mode;
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	if (cmd_args[i] == NULL)
	{
		printf("\n");
		return (0);
	}	
	if (ft_strncmp(cmd_args[i], "-n", 3) == 0)
	{
		new_line = 0;
		i++;
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
	(void)cmd_args;
	(void)sh;
	(void)mode;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	bi_cd(char **cmd_args, t_shell *sh, int mode)
{
	(void)sh;
	(void)mode;

	if (cmd_args[1] == NULL)
		return (0);
	if (chdir(cmd_args[1]) == -1)
		perror_exit("chdir");
	return (0);
}

int	bi_exit(char **cmd_args, t_shell *sh, int mode)
{
	int argc;
	int code;

	argc = get_arr_size(cmd_args);
	if (mode == EXEC_AS_PARENT)
	{
		ft_putstr_fd("exit\n", 2);
		if (argc > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		if (argc == 2)
		{
			code = ft_atoi(cmd_args[1]);
			free_arr(cmd_args);
			free_data(sh);
			exit(code);
		}
		if (argc == 1)
		{
			free_arr(cmd_args);
			free_data(sh);
			exit(EXIT_SUCCESS);
		}
	}
	if (mode == EXEC_AS_CHILD)
	{
		if (argc > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			exit(EXIT_FAILURE);
		}
		if (argc == 2)
		{
			code = ft_atoi(cmd_args[1]);
			free_arr(cmd_args);
			free_data(sh);
			exit(code);
		}
		if (argc == 1)
		{
			free_arr(cmd_args);
			free_data(sh);
			exit(EXIT_NO_ARG);
		}
	}
	return (0);
}

int	bi_env(char **cmd_args, t_shell *sh, int mode)
{
	(void)mode;
	int	argc;
	t_env_node	*runner;	
	
	argc = get_arr_size(cmd_args);
	if (argc > 1)
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	runner = sh->env->head;
	if (runner == NULL)
		return (0);
	while (runner)
	{
		printf("%s=%s\n", runner->key, runner->value);
		runner = runner->next;
	}	
	return (0);
}

int	bi_export(char **cmd_args, t_shell *sh, int mode)
{
	(void)mode;
	int	argc;
	t_env		*tmp;
	t_env_node	*runner;	
	
	argc = get_arr_size(cmd_args);
	tmp = env_dup(sh->env);
	if (argc == 1)
	{
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
				printf("declare -x %s=\"%s\"\n", runner->key, runner->value);
			runner = runner->next;
		}	
		free_env(tmp);
	}
// Error if identifier doesn't begin with alphabet or underscore
	return (0);
}

int	bi_unset(char **cmd_args, t_shell *sh, int mode)
{
	(void)mode;
	t_env		*tmp;
	
// Go through all arguments
// Print error if not valid identifier but continue with other variables
	tmp = env_dup(sh->env);
	remove_env_node(tmp, cmd_args[1]);
	free_env(tmp);
// Error if identifier doesn't begin with alphabet or underscore
	return (0);
}


// Case in pipe:
// - don't print exit in any case
// - print too many arguments if argc is not correct

// - exit code with 1 argument is argument
// - exit code with wrong number of arguments is 1
// - exit code without argument is exit code of last pipeline

// Case in single command:
// print exit in any case
// print too many arguments if argc is not correct and don't exit

// Exit code with 1 argument is argument
// exit code with wrong number of arguments is 1
// Exit code if no argument is 0

// Exit shell only if correct num of arguments

// free args at exit
// free sh?????????


// User Defined Exit Codes From 79-113 are okay
