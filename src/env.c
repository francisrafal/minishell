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
char **get_env_arr(t_env *env)
{
	t_env_node	*runner;
	char		**env_arr;
	char		*tmp;
	int			env_size;
	int			i;

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

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	int		size;
	char	*pwd;
	char	*cwd;

	if (envp == NULL)
		size = 0;
	else
		size = get_arr_size(envp);
	env = malloc (sizeof (t_env));
	if (env == NULL)
		return (NULL);
	env->head = NULL;
	env->size = 0;
	i = 0;
	while (i < size)
		append_env(env, create_env_node(envp[i++]));
	if (size == 0)
	{
		cwd = getcwd(NULL, 0);
		if (cwd == NULL)
			perror_exit("getcwd");
		pwd = ft_strjoin("PWD=", cwd);
		cwd = free_null(cwd);
		append_env(env, create_env_node(pwd));
		pwd = free_null(pwd);
	}
	return (env);
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

t_env_node	*append_env(t_env *env, t_env_node *node)
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
	return (node);
}

void	prepend_env(t_env *env, t_env_node *node)
{
	t_env_node	*tmp;

	tmp = env->head;
	env->head = node;
	node->next = tmp;
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

void	remove_env_node(t_env *env, char *key)
{
	t_env_node	*runner;
	t_env_node	*tmp;

	if (env == NULL)
		return ;
	if (env->head == NULL)
		return ;
	runner = env->head;
	if (ft_strncmp(key, runner->key, ft_strlen(runner->key) + 1) == 0)
	{
		tmp = runner;
		env->head = runner->next;
		runner = free_env_node_null(runner);
		return ;
	}
	while (runner->next && ft_strncmp(key, runner->next->key, ft_strlen(runner->next->key) + 1))
		runner = runner->next;
	if (runner->next)
	{
		tmp = runner->next;
		runner->next = runner->next->next;
		tmp = free_env_node_null(tmp);
		env->size--;
	}
}

void	insert_env_node_after_key(t_env *env, char *key, t_env_node *node)
{
	t_env_node	*runner;
	t_env_node	*tmp;

	if (env == NULL)
		return ;
	if (env->head == NULL || key == NULL)
		return ;
	runner = env->head;
	while (runner && ft_strncmp(key, runner->key, ft_strlen(runner->key) + 1))
		runner = runner->next;
	if (runner)
	{
		tmp = runner->next;
		runner->next = node;
		node->next = tmp;
		env->size++;
	}
}

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
			if (ft_strncmp(runner->key, runner->next->key, ft_strlen(runner->key) + 1) > 0)
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
	t_env_node *runner;

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
