#include "minishell.h"

char	**fill_env_arr(t_env *env, char **env_arr)
{
	t_env_node	*runner;
	int			i;
	char		*tmp;

	runner = env->head;
	i = 0;
	while (runner)
	{
		if (runner->value)
		{
			tmp = ft_strjoin(runner->key, "=");
			env_arr[i] = ft_strjoin(tmp, runner->value);
			tmp = free_null(tmp);
			i++;
		}
		runner = runner->next;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

char	**get_env_arr(t_env *env)
{
	t_env_node	*runner;
	char		**env_arr;
	int			env_size;

	if (env == NULL || env->head == NULL)
		return (NULL);
	env_size = 0;
	runner = env->head;
	while (runner)
	{
		if (runner->value)
			env_size++;
		runner = runner->next;
	}
	env_arr = malloc((env_size + 1) * sizeof (char *));
	if (env_arr == NULL)
		return (NULL);
	env_arr = fill_env_arr(env, env_arr);
	return (env_arr);
}

void	print_arr(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	printf("----\n");
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}	
}
