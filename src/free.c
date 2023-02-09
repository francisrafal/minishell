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
		tmp = runner;
		runner = tmp->next;
		free_env_node(tmp);
	}	
	free_null(env);
}

void	free_env_node(t_env_node *node)
{
	free_null(node->key);
	free_null(node->value);
	free_null(node);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free_null(arr[i++]);
	if (arr)
		free_null(arr);
}

void	free_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_lst(t_cmd **lst)
{
	t_cmd	*node;
	t_cmd	*next;

	node = *lst;
	if (node)
	{
		while (node)
		{
			next = node->next;
			if(node->delim)
				free(node->delim);
			free_arr(node->path);
			free_arr(node->opt);
			free_null(node);
			node = next;
		}
	}
	*lst = NULL;
}

/*void    free_cmd(t_cmd *cmd)
{
        if(cmd)
        {
                if(cmd->delim)
                        free(cmd->delim);
                free_lst(cmd->lst_cmds);
                free_null(cmd);

        }
        cmd = NULL;
}*/
