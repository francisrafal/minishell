#include "minishell.h"

char	*get_cmd_line(void)
{
	char *cmd_line;

	cmd_line = readline(BOLDYELLOW PROMPT RESET);
	return (cmd_line);
}
