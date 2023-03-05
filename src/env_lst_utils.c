#include "minishell.h"

void	sort_env(t_env *env)
{
	t_env_node	*runner;
	int			swap;

	if (env == NULL)
		return ;
	runner = env->head;
	if (runner == NULL || runner->next == NULL)
		return ;
	swap = 1;
	while (swap)
	{
		swap = 0;
		while (runner->next)
		{
			if (ft_strncmp(runner->key, runner->next->key,
					ft_strlen(runner->key) + 1) > 0)
			{
				runner = env_node_swap(env, runner->key);
				swap = 1;
			}
			runner = runner->next;
		}
		runner = env->head;
	}
}

t_env_node	*env_node_swap(t_env *env, char *key)
{
	t_env_node	*runner;	
	t_env_node	*new_runner;
	t_env_node	*tmp;

	runner = env->head;
	while (runner->next && ft_strncmp(runner->key, key, ft_strlen(key) + 1))
		runner = runner->next;
	new_runner = runner->next;
	tmp = env_node_dup(runner);
	tmp->next = runner->next;
	remove_env_node(env, runner->key);
	insert_env_node_after_key(env, tmp->next->key, tmp);
	return (new_runner);
}

t_env_node	*find_env_node(t_env *env, char *key)
{
	t_env_node	*runner;

	if (env == NULL || env->head == NULL)
		return (NULL);
	runner = env->head;
	while (runner && ft_strncmp(runner->key, key, ft_strlen(key) + 1))
		runner = runner->next;
	return (runner);
}

t_env_node	*replace_node_value(t_env_node *node, char *value)
{
	if (node == NULL)
		return (NULL);
	node->value = free_null(node->value);
	node->value = ft_strdup(value);
	return (node);
}
