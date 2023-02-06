#include "minishell.h"

void	exec_builtin(char **cmd_args)
{
	if (ft_strncmp(cmd_args[0], "echo", 5) == 0)
		echo(cmd_args);
	else if (ft_strncmp(cmd_args[0], "pwd", 4) == 0)
		pwd(cmd_args);
	else if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		cd(cmd_args);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		exit_shell(cmd_args);
	exit(EXIT_SUCCESS);
}

int	echo(char **cmd_args)
{
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

int	pwd(char **cmd_args)
{
	(void)cmd_args;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	printf("%s\n", cwd);
	return (0);
}

int	cd(char **cmd_args)
{
	if (cmd_args[1] == NULL)
		return (0);
	if (chdir(cmd_args[1]) == -1)
		perror_exit("chdir");
	return (0);
}

int	exit_shell(char **cmd_args)
{
	free_arr(cmd_args);
	printf("exit\n");
	exit(SHELL_EXIT);
}
