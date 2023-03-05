/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 12:03:23 by celgert           #+#    #+#             */
/*   Updated: 2023/03/05 14:05:00 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <fcntl.h>
# include <signal.h>

/* Project Headers */
# include "libft.h"

/* Macros */
# define BOLDYELLOW "\e[1;33m"
# define RESET "\e[0m"
# define PROMPT "minishell$ "
# define HERE_DOC "here_doc> "
# define EXIT_NO_ARG 79
# define EXEC_AS_PARENT 1
# define EXEC_AS_CHILD 2

/* Global Variables */
extern int	g_exit_code;

/* Structs */

/* Struct For Shell State */
typedef struct s_shell
{
	int				wstatus;
	struct s_env	*env;
	int				stdin_copy;
	pid_t			*pid;
}	t_shell;

/* Struct for each command of one pipe */
typedef struct s_cmd
{
	int				re_in;
	int				re_out;
	int				read_in;
	char			*delim;
	int				re_out_app;
	int				fd_in;
	int				fd_out;
	int				cmd_id;
	int				ncmds;
	char			**path;
	char			**opt;
	struct s_cmd	*next;
}		t_cmd;

/* Struct For Environment Node */
typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

/* Struct For Environment List */
typedef struct s_env
{
	struct s_env_node	*head;
	int					size;
}	t_env;

/* Enums */
typedef enum e_pipe_state
{
	START,
	CMD,
	BLANK,
	PIPE,
	MULTIPLE_PIPES,
	BEGIN_PIPE,
	REJECT
}	t_pipe_state;

/* Functions */

/* prompt.c */
char		*get_cmd_line(void);

/* builtins.c */
int			bi_echo(char **cmd_args, t_shell *sh, int mode);
int			bi_pwd(char **cmd_args, t_shell *sh, int mode);
int			bi_env(char **cmd_args, t_shell *sh, int mode);
int			bi_unset(char **cmd_args, t_shell *sh, int mode);

/* builtin_cd.c */
int			bi_cd(char **cmd_args, t_shell *sh);
void		update_pwd_and_oldpwd(t_shell *sh);

/* builtin_exit.c */
int			bi_exit(t_cmd *cmd, t_shell *sh, int mode);

/* builtin_export.c */
int			bi_export(char **cmd_args, t_shell *sh, int mode);
int			bi_export_no_args(char **cmd_args, t_shell *sh, int mode);
int			bi_export_with_args(char **cmd_args, t_shell *sh,
				int mode, int argc);

/* builtin_utils.c */
int			get_arr_size(char **arr);
void		print_with_escaped_quotes(char *key, char *value);

/* exec.c */
int			exec_builtin(t_cmd *cmd, t_shell *sh, int mode);
int			is_builtin(t_cmd *cmd);
t_cmd		*exec_parsed_cmd_line(t_cmd *cmd, t_shell *sh);
int			execve_safe(t_cmd *cmd, t_shell *sh);

/* exec_pipeline.c */
void		*exec_pipeline(t_cmd *cmd, t_shell *sh);
int			child_process_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh);
int			redirect_fds_pipeline(int *pipefd, t_cmd *cmd, t_shell *sh);
int			parent_process_pipeline(int *pipefd, t_cmd *cmd);
int			pipeline_loop(int *pipefd, t_cmd *cmd, t_shell *sh, int i);

/* exec_single_cmd.c */
void		*exec_single_cmd(t_cmd *cmd, t_shell *sh);
void		*exec_one_child(t_cmd *cmd, t_shell *sh);
int			child_process_single_cmd(t_cmd *cmd, t_shell *sh);
int			redirect_fds_single_cmd(t_cmd *cmd, t_shell *sh);

/* exec_utils.c */
char		*get_cmd_path(t_cmd *cmd);
void		append_str(char ***paths, char *str);

/* exits.c */
void		exit_after_failed_exec(t_cmd *cmd, t_shell *sh);
void		exit_on_file_error(t_cmd *cmd, t_shell *sh);

/* list_utils.c */
t_cmd		*ft_lstnew(int ncmds);
int			ft_lstsize(t_cmd *lst);
void		ft_lstadd_back(t_cmd **lst, t_cmd *new);
void		ft_display_lst(t_cmd *lst);

/* free.c */
void		*free_arr_null(char **arr);
void		*free_null(void *ptr);
void		*free_shell_null(t_shell *sh);
void		*free_env_null(t_env *env);
void		*free_env_node_null(t_env_node *node);

/* cmd_list_utils.c */
void		*free_lst_null(t_cmd *node);
void		*free_cmd_null(t_cmd *node);

/* split_input.c*/
int			split_line(t_cmd **cmd, const char *str, int ncmds, t_env *env);
t_cmd		*split_input_cmd(char **cmds, int ncmds,
				t_cmd *lst_cmds, t_env *env);
t_cmd		*split_input(char *line, t_env *env);

/* check_input.c and check_input2.c*/
int			check_syntax(char **cmds);
int			check_pipes(char *str);
int			check_quotes(char *str);

/* split_input_utils.c */
int			get_end_quote(char *line, char c);
int			count_cmds(char *line, char c);
char		*fill_cmds(char *line, int len);
char		**split_char(char *line, int *ncmds, char c);

/* files_utils.c */
int			get_next_char(char *line, char *cset);
int			get_char(char *line, char c);
char		*cut_word(char *str, char c);
char		*get_file_name(char *str, char c);

/* get_outfile.c */
int			open_close_outfile(char *tmp, char *file, t_cmd *cmd);
char		*get_outfile(t_cmd *cmd, char *line);

/* get_infile.c */
char		*get_heredoc_name(int cmd_id);
char		*get_infile(t_cmd *cmd, char *line);

/* prun_str.c */
char		*prun_str(char *str);

/* cut_char.c and cut_char_utils.c*/
int			check_str_env(char *str, t_env *env, int k);
char		*get_str_env(char *str, t_env *env, int k);
int			count_chars(char *str, t_env *env);
char		*replace_chars(char *str, t_env *env);

/* get_command.c */
char		*get_command(t_cmd *cmd, char *cmd_str, t_env *env);

/* error.c */
void		perror_exit(char *err);
int			ft_error(char *str, char *err);
void		print_exec_error(t_cmd *cmd);

/* env_arr.c */
char		**fill_env_arr(t_env *env, char **env_arr);
char		**get_env_arr(t_env *env);
void		print_arr(char **arr);

/* env.c */
t_env		*init_empty_env(t_env *env);
t_env		*init_env(char **envp);
t_env_node	*create_env_node(char *str);
t_env_node	*append_env(t_env *env, t_env_node *node);
void		prepend_env(t_env *env, t_env_node *node);

/* env_lst_utils2.c */
t_env		*env_dup(t_env *env);
t_env_node	*env_node_dup(t_env_node *node);
void		remove_env_node(t_env *env, char *key);
void		insert_env_node_after_key(t_env *env, char *key, t_env_node *node);

/* env_lst_utils.c */
void		sort_env(t_env *env);
t_env_node	*env_node_swap(t_env *env, char *key);
t_env_node	*find_env_node(t_env *env, char *key);
t_env_node	*replace_node_value(t_env_node *node, char *value);

/* init.c */
void		init_idx(int *arr, int len);
t_shell		*init_shell(char **envp);
void		increase_shell_level(t_env *env);
pid_t		*init_pid(t_shell *sh, int ncmds);

/* signals.c */
void		handle_signal_parent(int signum);
void		handle_signal_child(int signum);
void		set_signal_action(int mode);

/* debug_helpers.c */
void		lsof(char *helper_message);

/* syscall_wrappers.c */
void		close_or_print_error(int fd);
int			dup2_or_print_error(int oldfd, int newfd);
void		unlink_heredoc(t_cmd *cmd);
pid_t		fork_or_print_error(void);
int			pipe_or_print_error(int pipefd[2]);

/* wait_for_children.c */
pid_t		*wait_for_children(t_shell *sh, int children);

#endif
