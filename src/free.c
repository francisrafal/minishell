#include "minishell.h"

void	*free_data_null(t_shell *sh)
{
	sh->env = free_env_null(sh->env);
	sh->pid = free_null(sh->pid);
	sh = free_null(sh);
	return (NULL);
}

void	*free_env_null(t_env *env)
{
	t_env_node	*runner;	
	t_env_node	*tmp;	

	if (env == NULL)
		return (NULL);
	runner = env->head;
	if (runner == NULL)
	{
		env = free_null(env);
		return (NULL);
	}
	while (runner)
	{
		tmp = runner;
		runner = tmp->next;
		tmp = free_env_node_null(tmp);
	}	
	env = free_null(env);
	return (NULL);
}

void	*free_env_node_null(t_env_node *node)
{
	if (node == NULL)
		return (NULL);
	node->key = free_null(node->key);
	node->value = free_null(node->value);
	node = free_null(node);
	return (NULL);
}

void	*free_arr_null(char **arr)
{
	int	i;

	if (arr == NULL)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		arr[i] = free_null(arr[i]);
		i++;
	}
	arr = free_null(arr);
	return (NULL);
}

void	*free_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	*free_lst_null(t_cmd *node)
{
	t_cmd	*next;

	if (node == NULL)
		return (NULL);
	while (node)
	{
		node->delim = free_null(node->delim);
		node->path = free_arr_null(node->path);
		node->opt = free_arr_null(node->opt);
		next = node->next;
		node = free_null(node);
		node = next;
	}
	return (NULL);
}
