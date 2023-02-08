#include "minishell.h"
/*
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
*/
char **get_env(t_env *head)
{
	(void)head;
	return (NULL);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	int		size;

	size = get_arr_size(envp);
	env = malloc (sizeof (t_env));
	if (env == NULL)
		return (NULL);
	env->head = NULL;
	env->size = 0;
	i = 0;
	while (i < size)
		append_env(env, create_env_node(envp[i++]));
	return (env);
}

void	print_env(t_env *env)
{
	t_env_node	*runner;	

	runner = env->head;
	if (runner == NULL)
		return ;
	while (runner)
	{
		printf("%s=%s\n", runner->key, runner->value);
		runner = runner->next;
	}	
}

t_env_node	*create_env_node(char *str)
{
	t_env_node 	*node;
	char		*equal_sign;
	char		*end_of_str;

	if (str == NULL)
		return (NULL);
	node = malloc(sizeof (t_env_node));
	if (node == NULL)
		return (NULL);
	equal_sign = ft_strchr(str, '=');
	end_of_str = ft_strchr(str, '\0');
	if (equal_sign == NULL)
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	else
	{
		node->key = ft_substr(str, 0, equal_sign - str);
		node->value = ft_substr(str, (equal_sign - str) + 1, end_of_str - (equal_sign + 1));
	}
	node->next = NULL;
	return (node);
}

void	append_env(t_env *env, t_env_node *node)
{
	t_env_node	*runner;	

	runner = env->head;
	if (runner == NULL)
		env->head = node;
	else
	{
		while (runner->next)
			runner = runner->next;
		runner->next = node;
	}
	env->size++;
}

t_env	*env_dup(t_env *env)
{
	t_env 		*env_copy;
	t_env_node	*runner;

	env_copy = malloc (sizeof (t_env));
	if (env_copy == NULL)
		return (NULL);
	env_copy->head = NULL;
	env_copy->size = 0;
	runner = env->head;
	while (runner)
	{
		append_env(env_copy, env_node_dup(runner));
		runner = runner->next;
	}
	return (env_copy);
}

t_env_node	*env_node_dup(t_env_node *node)
{
	t_env_node 	*dup;

	if (node == NULL)
		return (NULL);
	dup = malloc(sizeof (t_env_node));
	if (dup == NULL)
		return (NULL);
	dup->key = ft_strdup(node->key);
	if (node->value == NULL)
		dup->value = NULL;
	else
		dup->value = ft_strdup(node->value);
	dup->next = NULL;
	return (dup);
}
