#ifndef MINISHELL_H
# define MINISHELL_H

/* C System Headers */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>

/* Project Headers */
# include "libft.h"

/* Macros */
# define BOLDYELLOW "\e[1;33m"
# define RESET "\e[0m"
# define PROMPT "minishell$ "
# define SHELL_EXIT 255

/* Structs */

/* Struct For Shell State */
typedef	struct s_shell
{
	int	exit;
	int	wstatus;
}	t_shell;

/* Struct For List Of Cmds */
typedef struct s_lst_cmds
{
	char	**path;
	char	**opt;
	int		fd;
	struct s_lst_cmds	*next;
}		t_cmds;

/* Struct For New Command From Minishell */
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
void	exec_builtin(char **cmd_args);
int		echo(char **cmd_args);
int		pwd(char **cmd_args);
int		cd(char **cmd_args);
int		exit_shell(char **cmd_args);

/* list_utils.c */
t_cmds	*ft_lstnew(int fd);
void	ft_lstadd_back(t_cmds **lst, t_cmds *new);
//void	ft_display_lst(t_cmds *lst);
void	free_arr(char **arr);
void	free_null(void *ptr);
void	free_lst(t_cmds **lst);

/* files_utils.c */

/* error.c */
void	perror_exit(char *err);

#endif
