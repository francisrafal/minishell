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
	while (1)
	{		
		set_signal_action(EXEC_AS_PARENT);
		cmd_line = get_cmd_line();
		if (cmd_line == NULL)
			continue ;
		add_history(cmd_line);
		cmd = split_input(cmd_line, sh->env);
		cmd_line = free_null(cmd_line);
		if (cmd == NULL)
			continue ;
		cmd = exec_parsed_cmd_line(cmd, sh);
	}
	return (0);
}
