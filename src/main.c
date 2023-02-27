#include "minishell.h"

int	g_exit_code = 0;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char	*cmd_line;
	char	**test_cmd; 
	int		num_cmds;
	t_shell	*sh;
	t_cmd	*cmd;
	sh = init_shell(envp);
	num_cmds = 1;
	cmd = NULL;
	while (1)
	{		
		set_signal_action(EXEC_AS_PARENT);
		cmd_line = get_cmd_line();
		if (cmd_line == NULL)
			cmd_line = ft_strdup("exit");
		if (cmd_line[0] == 0)
		{
			free_null(cmd_line);
			continue ;
		}
		add_history(cmd_line);
		//cmd = split_input(cmd_line, get_env_arr(sh->env));
		cmd = split_input(cmd_line, sh->env);
		test_cmd = ft_split(cmd_line, ' ');
		free_null(cmd_line);
		if (num_cmds == 1)
			exec_single_cmd(test_cmd, sh);
		else
			exec_pipeline(cmd, sh);
		free_arr(test_cmd);
		free_lst(&cmd);	
	}
	return (0);
}
