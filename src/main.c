#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*cmd_line;
	char	**test_cmd; 

	while (1)
	{		
		cmd_line = get_cmd_line();
		test_cmd = ft_split(cmd_line, ' ');
		exec_builtin(test_cmd);
		free(cmd_line);
		free_arr(test_cmd);	
	}
	return (0);
}
