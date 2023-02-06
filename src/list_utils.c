#include "minishell.h"

t_cmds	*ft_lstnew(int fd)
{
	t_cmds	*elem;

	elem = (t_cmds *)malloc(sizeof(t_cmds));
	if (elem)
	{
		elem->next = NULL;
		elem->path = NULL;
		elem->opt = NULL;
		elem->fd = fd;
	}
	return (elem);
}

void	ft_lstadd_back(t_cmds **lst, t_cmds *new)
{
	t_cmds	*node;

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

/*void	ft_display_lst(t_cmds *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		printf("Command: |%s|\n", lst->opt[0]);
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
		printf("File descriptor: |%d|\n", lst->fd);
		printf("\n");
		lst = lst->next;
	}
}
*/
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

void	free_lst(t_cmds **lst)
{
	t_cmds	*node;
	t_cmds	*next;

	node = *lst;
	if (node)
	{
		while (node)
		{
			next = node->next;
			free_arr(node->path);
			free_arr(node->opt);
			free_null(node);
			node = next;
		}
	}
	*lst = NULL;
}
