#include "minishell.h"

void	exec_builtin(char **cmd_args, t_shell *sh, int mode)
{
	if (ft_strncmp(cmd_args[0], "echo", 5) == 0)
		bi_echo(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "pwd", 4) == 0)
		bi_pwd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		bi_cd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		bi_exit(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "env", 4) == 0)
		bi_env(cmd_args, sh, mode);
}

void	exec_as_parent(char **cmd_args, t_shell *sh)
{
	exec_builtin(cmd_args, sh, EXEC_AS_PARENT);
}

void	exec_as_child(char **cmd_args, t_shell *sh)
{
	if (fork() == 0)
	{
		exec_builtin(cmd_args, sh, EXEC_AS_CHILD);
		free_arr(cmd_args);
		free_data(sh);
		exit(EXIT_SUCCESS);
	}
	else
	{	
		wait(&sh->wstatus);
		// Replace here with waitpid when we have multiple cmds
		// Later: EXIT_NO_ARG is used so that $? is set properly. if EXIT_NO_ARG then $? should remain the value of the last pipeline
		if (WEXITSTATUS(sh->wstatus) == EXIT_NO_ARG)
			sh->exit = 1;
	}
}

void	exec_single_cmd(char **cmd_args, t_shell *sh)
{
	// Change this function later so that it takes t_cmd *cmd as input instead of char **cmd_args
	if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		exec_as_parent(cmd_args, sh);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		exec_as_parent(cmd_args, sh);
	else
		exec_as_child(cmd_args, sh);
}

void	exec_pipeline(t_cmd *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	// exec pipeline in here
} 
