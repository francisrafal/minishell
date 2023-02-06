#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*cmd_line = get_cmd_line();
	// Test echo
	char	**echo_test_cmd = ft_split(cmd_line, ' ');
	echo(echo_test_cmd);

	free(cmd_line);
	// echo_test_cmd must be freed too
	return (0);
}
