static t_cmd	*init_input_cmd(void)
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



t_cmd	*split_input_cmd(char *line, char **envp)
{
	t_cmd	*comd;
	
	comd = init_input_cmd();
	if (!comd)
		free_cmd(comd);


}
