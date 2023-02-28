#include "minishell.h"

void	init_idx(int *arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		arr[i] = 0;
		i++;
	}
}

t_shell	*init_shell(char **envp)
{
	t_shell	*sh;

	sh = malloc(sizeof (t_shell));
	if (sh == NULL)
		perror_exit("malloc");
	sh->env = init_env(envp);
	increase_shell_level(sh->env);
	sh->exit = 0;
	sh->stdin_copy = 0;
	return (sh);
}

void	increase_shell_level(t_env *env)
{
	t_env_node	*node;
	char		*new_shell_level;

	node = find_env_node(env, "SHLVL");
	if (node)
	{
		new_shell_level = ft_itoa(ft_atoi(node->value) + 1);
		replace_node_value(node, new_shell_level);
		free_null(new_shell_level);
	}
	else
	{
		new_shell_level = "SHLVL=1";
		append_env(env, create_env_node(new_shell_level));
	}
}
