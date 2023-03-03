#include "minishell.h"

void	lsof(char *helper_message)
{
	char	cmd1[100];
	char	cmd2[100];

	system("echo '--------------------------' >&2");
	sprintf(cmd1, "echo '%s' >&2", helper_message);
	system(cmd1);
	sprintf(cmd2, "lsof -a -d 0-999 -p %d >&2", getpid());
	system(cmd2);
	system("echo '--------------------------' >&2");
}
