#include "minishell.h"

t_env	*init_empty_env(t_env *env)
{
	char	*pwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		perror_exit("getcwd");
	pwd = ft_strjoin("PWD=", cwd);
	cwd = free_null(cwd);
	append_env(env, create_env_node(pwd));
	pwd = free_null(pwd);
	return (env);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	int		size;

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
		env = init_empty_env(env);
	return (env);
}

t_env_node	*create_env_node(char *str)
{
	t_env_node	*node;
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
		node->value = ft_substr(str, (equal_sign - str) + 1,
				end_of_str - (equal_sign + 1));
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
