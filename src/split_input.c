#include "minishell.h"

/*static t_cmd	*init_input_cmd(void)
{
	t_cmd   *cmd;

        cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if(cmd)
	{
		cmd->re_in = 0;
		cmd->re_out = 0;
		cmd->read_in = 0;
		cmd->delim = "";
		cmd->fd_in = 0;
		cmd->fd_out = 1;
		cmd->lst_cmds = NULL;
	}
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	if(cmd)
	{
		if(cmd->delim)
			free(cmd->delim);
		free_lst(cmd->lst_cmds);
		free_null(cmd);

	}
	cmd = NULL;
}
*/

void	split_line(t_cmd **lst_cmds, const char *str)
{
//	int	i;
	(void)lst_cmds;
	t_cmd	*cmd;
	char	*line;

	cmd = ft_lstnew();
	line = ft_strdup(str);
	printf("%s\n", line);
	//get_infile(cmd, line);
	//get_outfile(cmd,line);
	//get_command(cmd,line);
	ft_lstadd_back(lst_cmds, cmd);
/*	i = 0; 
	while (line[i])
	{
		//while (str[i] && (str[i] == '\t'  || str[i] == ' '))
		//	i++;
		if(ft_isalpha(str[i]))
		{
			printf("command\n");
		//	get_cmd(str, &i);
		}
		else if(str[i] == '<')
		{
			printf("file in\n");
		//	get_infile(str, &i);
		}
		else if(str[i] == '>')
		{
			printf("file out\n");
		//	get_outfile(str, &i);
		}

		printf("%c", str[i]);
		i++;		
	}*/
}


t_cmd   *split_input_cmd(char *line, char **envp)
{
        t_cmd   *lst_cmds;
	char  **cmds;
        (void)envp;
	int i;
	
	lst_cmds = (t_cmd *)malloc(sizeof(t_cmd));
        lst_cmds = NULL;
	cmds = split_pipes(line);
	i = 0;
	while (cmds[i])
	{
		//printf("|%s|\n", cmds[i]);
		split_line(&lst_cmds, cmds[i]);
		i++;
	}
       //comd= NULL;
	/* comd = init_input_cmd();
        if (!comd)
                free_cmd(comd);
	
        split_line(comd, line);
//      free_cmd(comd);*/
	ft_display_lst(lst_cmds);
	free_arr(cmds);
        return (lst_cmds);

}

