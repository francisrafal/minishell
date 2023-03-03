#include "minishell.h"

t_cmd	*ft_lstnew(int ncmds)
{
	t_cmd	*elem;

	elem = (t_cmd *)malloc(sizeof(t_cmd));
	if (!elem)
		return (NULL);
	elem->re_in = 0;
	elem->re_out = 0;
	elem->read_in = 0;
	elem->delim = NULL;
	elem->fd_in = 0;
	elem->fd_out = 1;
	elem->ncmds = ncmds;
	elem->next = NULL;
	elem->path = NULL;
	elem->opt = NULL;
	return (elem);
}

void	ft_lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*node;

	node = *lst;
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new;
	}
	else
		*lst = new;
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
/*
void	ft_display_lst(t_cmd *lst)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (lst != NULL)
	{
		printf("Command %i of %i cmds: \n", j+1, lst->ncmds); // |%s|\n", lst->opt[0]);
		if (!lst->path)
			printf("No paths!\n");
		else
		{
			while (lst->path[i])
			{
				printf("Path %d: %s\n", i, lst->path[i]);
				i++;
			}
		}
		i = 0;
		if (!lst->opt)
			printf("No options!\n");
		else
		{
			while (lst->opt[i])
			{
				printf("Options  %d: |%s|\n", i, lst->opt[i]);
				i++;
			}
		}
		printf("File descriptor for input: |%d|\n", lst->fd_in);
		printf("File descriptor for output: |%d|\n", lst->fd_out);
		printf("\n");
		lst = lst->next;
		j++;
	}
}*/
