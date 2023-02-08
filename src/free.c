#include "minishell.h"

void	free_data(t_shell *sh)
{
	free_env(sh->env);
	free_null(sh);
}

void	free_env(t_env *env)
{
	t_env_node	*runner;	
	t_env_node	*tmp;	

	runner = env->head;
	if (runner == NULL)
	{
		free_null(env);
		return ;
	}
	while (runner)
	{
		free_null(runner->key);
		free_null(runner->value);
		tmp = runner;
		runner = runner->next;
		free_null(tmp);
	}	
	free_null(env);
}
