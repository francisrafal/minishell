#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*cmd_line;
	char	**echo_test_cmd; 

	while (1)
	{		
		cmd_line = get_cmd_line();
		// Test echo
		echo_test_cmd = ft_split(cmd_line, ' ');
		echo(echo_test_cmd);
		free(cmd_line);
		free_arr(echo_test_cmd);	
	}
	return (0);
}
