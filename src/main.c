#include "minishell.h"

int	g_exit_code = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*cmd_line;
	t_shell	*sh;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;

	sh = init_shell(envp);
	cmd = NULL;
	while (1)
	{		
		set_signal_action(EXEC_AS_PARENT);
		cmd_line = get_cmd_line();
		if (cmd_line == NULL)
			cmd_line = ft_strdup("exit");
		if (cmd_line[0] == 0)
		{
			cmd_line = free_null(cmd_line);
			continue ;
		}
		add_history(cmd_line);
		cmd = split_input(cmd_line, sh->env);
		if (!cmd)
		{
			cmd_line = free_null(cmd_line);
			printf("syntax error\n"); // write this to STDERR
			continue ;
		}
		//ft_display_lst(cmd);
		cmd_line = free_null(cmd_line);
		if (cmd->ncmds == 1)
			exec_single_cmd(cmd, sh);
		else
		{
			set_signal_action(EXEC_AS_CHILD);
			exec_pipeline(cmd, sh);
		}
		cmd = free_lst_null(cmd);
	}
	return (0);
}
