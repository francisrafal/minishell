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
# define EXIT_NO_ARG 79
# define EXEC_AS_PARENT 1
# define EXEC_AS_CHILD 2

/* Structs */

/* Struct For Shell State */
typedef	struct s_shell
{
	int				exit;
	int				wstatus;
	int				last_exit_status;
	struct s_env	*env;
}	t_shell;

/* Struct For List Of Cmds */
typedef struct s_lst_cmds
{
	char	**path;
	char	**opt;
	int		fd;
	struct s_lst_cmds	*next;
}		t_cmds;

/* Struct for each command of one pipe */
typedef struct s_cmd
{
	int			re_in;
	int			re_out;
	int			read_in;
	char		*delim;
	int			re_out_app;
	int			fd_in;
	int			fd_out;
	int 			ncmds;
	char 		**path;
	char		**opt;
	struct s_cmd	*next;
//	t_cmds		**lst_cmds;
}		t_cmd;

/* Struct For Environment Node */
typedef struct s_env_node
{
	char 				*key;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

/* Struct For Environment List */
typedef struct s_env
{
	struct s_env_node	*head;
	int					size;
}	t_env;

/* Functions */

/* prompt.c */
char	*get_cmd_line(void);

/* builtins.c */
int		bi_echo(char **cmd_args, t_shell *sh, int mode);
int		bi_pwd(char **cmd_args, t_shell *sh, int mode);
int		bi_cd(char **cmd_args, t_shell *sh, int mode);
int		bi_exit(char **cmd_args, t_shell *sh, int mode);
int		bi_env(char **cmd_args, t_shell *sh, int mode);
int		bi_export(char **cmd_args, t_shell *sh, int mode);
int		bi_unset(char **cmd_args, t_shell *sh, int mode);

/* builtin_utils.c */
int		get_arr_size(char **cmd_args);

/* exec.c */
int		exec_builtin(char **cmd_args, t_shell *sh, int mode);
void	exec_as_parent(char **cmd_args, t_shell *sh);
void	exec_as_child(char **cmd_args, t_shell *sh);
void	exec_single_cmd(char **cmd_args, t_shell *sh);
void	exec_pipeline(t_cmd *cmd, t_shell *sh);

/* list_utils.c */
t_cmd	*ft_lstnew(void); //int fd);
void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
void	ft_display_lst(t_cmd *lst);

/* free.c */
void	free_arr(char **arr);
void	free_null(void *ptr);
void	free_lst(t_cmd **lst);
//void    free_cmd(t_cmd *cmd);
void	free_data(t_shell *sh);
void	free_env(t_env *env);
void	free_env_node(t_env_node *node);

/* split_input.c*/
void	split_line(t_cmd **cmd, const char *str);
t_cmd	*split_input_cmd(char *line, char **envp);

/* split_input_utils.c */
int     get_end_quote(char *line, char c);
int     count_cmds(char *line);
char    *fill_cmds(char *line, int len);
void    init_idx(int *arr, int len);
char    **split_pipes(char *line);

/* files_utils.c */

/* error.c */
void	perror_exit(char *err);

/* env.c */
//char		**copy_env(char **envp);
char 		**get_env(t_env *head);
t_env		*init_env(char **envp);
t_env_node	*create_env_node(char *str);
void		append_env(t_env *env, t_env_node *node);
void		prepend_env(t_env *env, t_env_node *node);
t_env		*env_dup(t_env *env);
t_env_node	*env_node_dup(t_env_node *node);
void		remove_env_node(t_env *env, char *key);
void		insert_env_node_after_key(t_env *env, char *key, t_env_node *node);
void		sort_env(t_env *env);
t_env_node	*env_node_swap(t_env *env, char *key);

/* init.c */
t_shell	*init_shell(char **envp);

#endif
