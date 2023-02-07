#include "minishell.h"

char	**copy_env(char **envp)
{
	char	**env;
	int		size;	
	int		i;

	size = 0;
	while (envp[size])
		size++;
	env = malloc(sizeof (char *) * (size + 1));
	if (env == NULL)
		perror_exit("malloc");	
	i = 0;
	while (i < size)
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}
