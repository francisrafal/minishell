// **cmd_args must be a null-terminated string-array
// cmd_args[0] is the command name "echo"
// cmd_args[1] and higher are the args to the cmd "echo" which are separated by blanks (spaces/tabs)
// cmd_args[1] can also be "-n"
// Example:
// echo This is a test string.
// Output:
// This is a test string.

#include "minishell.h"

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
