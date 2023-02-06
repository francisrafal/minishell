#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char	*cmd_line;
	char	**test_cmd; 
	t_shell	sh;

	sh.env = copy_env(envp);
	sh.exit = 0;
	while (1)
	{		
		cmd_line = get_cmd_line();
		test_cmd = ft_split(cmd_line, ' ');
		free(cmd_line);
		if (fork() == 0)
			exec_builtin(test_cmd, sh.env);
		else
		{	
			free_arr(test_cmd);	
			wait(&sh.wstatus);
			if (WEXITSTATUS(sh.wstatus) == SHELL_EXIT)
				sh.exit = 1;
		}
		if (sh.exit)
		{
			free_arr(sh.env);
			exit(EXIT_SUCCESS);
		}
	}
	return (0);
}
