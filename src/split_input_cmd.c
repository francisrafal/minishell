#include "minishell.h"

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

void	split_line(t_cmd *cmd, char *str)
{
	int	i;
	(void)cmd;

	i = 0; 
	while (str[i])
	{
		while (str[i] && (str[i] == '\t'  || str[i] == ' '))
			i++;
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
	}
}


int	get_end_quote(char *line, char c)
{
	int	i;

	i = 0;
	while(line[i])
	{
		if(line[i] == c)
		{
			return(i);
		}
		i++;
	}
	return(i);
}

int 	count_cmds(char *line)
{
	int	i;
	int	ncmds;

	ncmds = 1;
	i = 0;
	while(line[i] && i < (int)ft_strlen(line))
	{
		if(line[i] == '"' && line[i+1])
		{
			i+=get_end_quote(&line[i+1],'"')+1;
		}
		else if(line[i] == '\'' && line[i+1])
			i+=get_end_quote(&line[i+1],'\'')+1;
		else if(line[i] == '|')
			ncmds +=1;
	    	if(line[i] && i!=(int)ft_strlen(line))	
			i++;
	}
	return (ncmds);
}

char	*fill_cmds(char *line, int len)
{
	char	*str;

	str=(char *)malloc(sizeof(char)*(len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, line, len + 1);
	return(str);
}

void	init_idx(int *arr, int len)
{
	int	i;

	i = 0;
	while(i < len)
	{
		arr[i] = 0;
		i++;
	}
}

char	**split_pipes(char *line)
{
	char	**cmds;
	int	len;
	int	*idx;

	idx  = NULL;
	len = count_cmds(line);
	printf("len : %i", len);
	return(NULL);	
	cmds = (char **)malloc(sizeof(char *) * (len + 1));
	init_idx(idx, 4);
	printf("counts cmds %i\n", len);
	return (NULL);
	while (line[idx[0]])
	{
		idx[4]=idx[0];
		if(line[idx[0]] == '"')
                        idx[0]=get_end_quote(&line[idx[0]],'"');
                else if(line[idx[0]] == '\'')
                        idx[0]=get_end_quote(&line[idx[0]],'\'');
                else if(line[idx[0]] == '|')
		{
			cmds[idx[2]]=fill_cmds(&line[idx[4]],idx[0]- idx[4]);
			idx[2]=+1;
		}
                if(line[idx[0]])
                        idx[0]+=1;
	}
	cmds[len + 1]=NULL;
	return(cmds);

}

t_cmd   *split_input_cmd(char *line, char **envp)
{
        t_cmd   *comd;
	char  **cmds;
        (void)envp;
	int i;

	cmds=split_pipes(line);
	i = 0;
	while (cmds[i])
	{
		printf("%s", cmds[i]);
		i++;
	}

        comd = init_input_cmd();
        if (!comd)
                free_cmd(comd);
	
        split_line(comd, line);
//      free_cmd(comd);

        return (comd);

}

