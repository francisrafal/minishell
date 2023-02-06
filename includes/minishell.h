#ifndef MINISHELL_H
# define MINISHELL_H

/* C System Headers */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

/* Project Headers */
# include "libft.h"

/*struct for list of cmds*/
typedef struct s_lst_cmds
{
	char	**path;
	char	**opt;
	int		fd;
	struct s_lst_cmds	*next;
}		t_cmds;

/*struct for new command from minishell*/
typedef struct s_cmd
{
	int			re_in;
	int			re_out;
	int			read_in;
	char		*delim;
	int			re_out_app;
	t_cmds		lst_cmds;
}		t_cmd;

/*commands in list_utils*/
t_cmds	*ft_lstnew(int fd);
void	ft_lstadd_back(t_cmds **lst, t_cmds *new);
//void	ft_display_lst(t_cmds *lst);
void	free_lst(t_cmds **lst);
#endif
