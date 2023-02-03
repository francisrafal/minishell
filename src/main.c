#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*cmd_line = get_cmd_line();
	printf("Your Command: %s\n", cmd_line);
	free(cmd_line);
	return (0);
}
