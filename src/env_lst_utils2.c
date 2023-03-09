/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 16:50:28 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:50:35 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_dup(t_env *env)
{
	t_env		*env_copy;
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
	t_env_node	*dup;

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
	while (runner->next
		&& ft_strncmp(key, runner->next->key, ft_strlen(runner->next->key) + 1))
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
