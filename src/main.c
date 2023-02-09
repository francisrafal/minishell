#include "minishell.h"

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
		cmd_line = get_cmd_line();
		split_input_cmd(cmd_line, envp);
		test_cmd = ft_split(cmd_line, ' ');
		free(cmd_line);
		if (num_cmds == 1)
			exec_single_cmd(test_cmd, sh);
		else
			exec_pipeline(cmd, sh);
		free_arr(test_cmd);	
	}
	return (0);
}
