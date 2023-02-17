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
char	*get_command(t_cmd *cmd, char *str)
{
	int	nopt;
	/*cmd->opt = ft_split(str, ' ');
	if (!cmd->opt)
	{
		free(str);
		return (NULL);
	}*/
	cmd->opt = split_char(str, &nopt, ' ');
	return (str);

}

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
	line = get_command(cmd,line);
	ft_lstadd_back(lst_cmds, cmd);
	free(line);
}

t_cmd	*split_input_cmd(char *line, char **envp)
{
	t_cmd	*lst_cmds;
	char	**cmds;
	int		i;
	int		ncmds;

	(void)envp;
       	cmds = split_char(line, &ncmds, '|');
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
