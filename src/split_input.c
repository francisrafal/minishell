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

void	split_line(t_cmd **lst_cmds, const char *str, int ncmds)
{
//	int	i;
	(void)lst_cmds;
	t_cmd	*cmd;
	char	*line;

	line = NULL;
	cmd = ft_lstnew(ncmds);
	line = ft_strdup(str);
	printf("|%s|\n", line);
	line = get_outfile(cmd, line);
	line = get_infile(cmd, line);
	//get_command(cmd,line);
	ft_lstadd_back(lst_cmds, cmd);
	free(line);
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

t_cmd	*split_input_cmd(char *line, char **envp)
{
	t_cmd	*lst_cmds;
	char	**cmds;
	int		i;
	int		ncmds;

	(void)envp;
       	cmds = split_pipes(line, &ncmds);
	/*lst_cmds = (t_cmd *)malloc(sizeof(t_cmd));
        if (!lst_cmds)
		return (NULL); */
	lst_cmds = NULL;
	if (!check_syntax(cmds))
	{
		i = 0;
		while (cmds[i])
		{
			//printf("%s\n", cmds[i]);
			split_line(&lst_cmds, cmds[i], ncmds);
			i++;
		}
		ft_display_lst(lst_cmds);
		free_lst(&lst_cmds);
		free_arr(cmds);
	}
	else	
	{
		free_arr(cmds);
		printf("TODO: here exit because of syntax error\n");
	}
        return (lst_cmds);
}
