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
	elem->cmd_id = 0;
	elem->ncmds = ncmds;
	elem->next = NULL;
	elem->path = NULL;
	elem->opt = NULL;
	return (elem);
}

int	ft_lstsize(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
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
