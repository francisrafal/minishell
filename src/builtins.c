#include "minishell.h"

int	bi_echo(char **cmd_args, char **env, int mode)
{
	(void)env;
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

int	bi_pwd(char **cmd_args, char **env, int mode)
{
	(void)cmd_args;
	(void)env;
	(void)mode;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	printf("%s\n", cwd);
	return (0);
}

int	bi_cd(char **cmd_args, char **env, int mode)
{
	(void)env;
	(void)mode;

	if (cmd_args[1] == NULL)
		return (0);
	if (chdir(cmd_args[1]) == -1)
		perror_exit("chdir");
	return (0);
}

int	bi_exit(char **cmd_args, char **env, int mode)
{
	if (mode == EXEC_AS_PARENT)
	{
		ft_putstr_fd("exit\n", 2);
		if (calc_argc(cmd_args) > 3)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
			// this should not exit the shell
		}
	}
	if (calc_argc(cmd_args) > 3)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	/*
	if (mode == EXEC_AS_PARENT)
	else
	{j
*/	
	free_arr(cmd_args);
	free_arr(env);
/*	if (

*/
	exit(SHELL_EXIT);
}


// Case in pipe:
// - don't print exit in any case
// - print too many arguments if argc is not correct
// - exit code with argument is argument
// - exit code without argument is exit code of last pipeline
// - exit code with wrong number of arguments is 1

// Case in single command:
// print exit in any case
// print too many arguments if argc is not correct and don't exit
// exit code if too many arguments is 1
// Exit code if no argument is 0
// Exit code if 1 argument is argument
// Exit shell only if correct num of arguments

// free args at exit
