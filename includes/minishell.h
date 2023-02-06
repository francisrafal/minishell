#ifndef MINISHELL_H
# define MINISHELL_H

/* C System Headers */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Project Headers */
# include "libft.h"

/* Macros */
# define BOLDYELLOW "\e[1;33m"
# define RESET "\e[0m"
# define PROMPT "minishell$ "

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
	int			fd_in;
	int			fd_out;
	t_cmds		**lst_cmds;
}		t_cmd;

/* Functions */

/* prompt.c */
char	*get_cmd_line(void);

/* builtins.c */
int	echo(char **cmd_args);

/* list_utils.c */
t_cmds	*ft_lstnew(int fd);
void	ft_lstadd_back(t_cmds **lst, t_cmds *new);
//void	ft_display_lst(t_cmds *lst);
void	free_lst(t_cmds **lst);

/* files_utils.c */

#endif
