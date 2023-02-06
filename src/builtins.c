#include "minishell.h"

void	exec_builtin(char **cmd_args, char **env)
{
	if (ft_strncmp(cmd_args[0], "echo", 5) == 0)
		echo(cmd_args, env);
	else if (ft_strncmp(cmd_args[0], "pwd", 4) == 0)
		pwd(cmd_args, env);
	else if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		cd(cmd_args, env);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		exit_shell(cmd_args, env);
	exit(EXIT_SUCCESS);
}

int	echo(char **cmd_args, char **env)
{
	(void)env;
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

int	pwd(char **cmd_args, char **env)
{
	(void)cmd_args;
	(void)env;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	printf("%s\n", cwd);
	return (0);
}

int	cd(char **cmd_args, char **env)
{
	(void)env;

	printf("%s\n", cmd_args[1]);
	if (cmd_args[1] == NULL)
		return (0);
	if (chdir(cmd_args[1]) == -1)
		perror_exit("chdir");
	// fix: cd alone should run in parent and not in child
	// cd in a pipeline runs in child
	return (0);
}

int	exit_shell(char **cmd_args, char **env)
{
	free_arr(cmd_args);
	free_arr(env);
	printf("exit\n");
	exit(SHELL_EXIT);
}
