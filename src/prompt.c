#include "minishell.h"

char	*get_cmd_line(void)
{
	char *cmd_line;

	cmd_line = readline(PROMPT);
	return (cmd_line);
}
