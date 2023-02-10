#include "minishell.h"

int	exec_builtin(char **cmd_args, t_shell *sh, int mode)
{
	if (ft_strncmp(cmd_args[0], "echo", 5) == 0)
		sh->last_exit_status = bi_echo(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "pwd", 4) == 0)
		sh->last_exit_status = bi_pwd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "cd", 3) == 0)
		sh->last_exit_status = bi_cd(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0)
		sh->last_exit_status = bi_exit(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "env", 4) == 0)
		sh->last_exit_status = bi_env(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "export", 7) == 0)
		sh->last_exit_status = bi_export(cmd_args, sh, mode);
	else if (ft_strncmp(cmd_args[0], "unset", 6) == 0)
		sh->last_exit_status = bi_unset(cmd_args, sh, mode);
	return (sh->last_exit_status);
}

void	exec_as_parent(char **cmd_args, t_shell *sh)
{
	exec_builtin(cmd_args, sh, EXEC_AS_PARENT);
}

void	exec_as_child(char **cmd_args, t_shell *sh)
{
	int	last_exit_status;
	if (fork() == 0)
	{
		last_exit_status = exec_builtin(cmd_args, sh, EXEC_AS_CHILD);
		free_arr(cmd_args);
		free_data(sh);
		exit(last_exit_status);
	}
	else
	{	
		// Replace here with waitpid when we have multiple cmds
		wait(&sh->wstatus);
		if (WIFEXITED(sh->wstatus))
			sh->last_exit_status = WEXITSTATUS(sh->wstatus);
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
	else if (ft_strncmp(cmd_args[0], "export", 7) == 0)
		exec_as_parent(cmd_args, sh);
	else if (ft_strncmp(cmd_args[0], "unset", 6) == 0)
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
