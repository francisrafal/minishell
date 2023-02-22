#include "minishell.h"

char	*replace_var(char *cmd, char **envp)
{
	char	*trim;
	(void)envp;

	trim = NULL;
	if (cmd[0] == '\"')
	{
		trim = ft_strtrim(cmd, "\"");
		free(cmd);
		printf("replace var is missing!\n");

	}
	else if (cmd[0] == '\'')
	{
		trim = ft_strtrim(cmd, "\'");
		free(cmd);
	}
	return (trim);
}



/*char	**cut_empty_str(char **arr)
{
	int	len;
	int	i;
	int	nempty;	
	char	**new;	
	i = 0;
	nempty = 0;
	while (arr[i])
	{
		if (arr[i][0] == '\0')
			nempty++;
		i++;
	}
	len = i;
	new = (char **)malloc(sizeof(char) * (len - nempty +1));
	if (! new)
	{	
		free_arr(arr);
		return (NULL);
	}
	i = 0;
	while (i < )
	printf("len array = %i, #empty str=%i\n", len, nempty);
	return (arr);
}*/
char	*cutted_spaces(char *str, char *new, char c)
{
	int	i; 
	int	j;
	int	k;
	
	i = 0;
	j = 1;
	k = 0;
	while (str[i])
	{
		new[k] = str[i];
		if (str[i] == c)
		{
			while (str[i+j] && str[i+j] == c)
				j++;
		}
		i += j;
		j=1;
		k++;
	}
	new[k] = '\0';
	return (new);
}

int     n_add_char(char *str, char c)
{
        int     i;
        int     j;
        int     k;

        i = 0;
        j = 1;
        k = 0;
        while (str[i])
        {
                if (str[i] == c)
                {
                        while (str[i+j] && str[i+j] == c)
                        {
                                j++;
                                k++;
                        }
                }
                i+= j+1;
                j=1;
        }
        return (k);
}




char *rm_space(char *str)
{
	int	len;
	int	nspace;
	char	*new;

	len = (int)ft_strlen(str);
	nspace = n_add_char(str, ' ');
	if (nspace == 0)
		return (str);
	new = (char *)malloc(sizeof(char) * (len - nspace + 1));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	new = cutted_spaces(str, new, ' ');
	free(str); 
	printf(" len %i | add space %i\n", len, nspace);
	return (new);
}

char	*get_command(t_cmd *cmd, char *cmd_str, char **envp)
{
	int	i;
	int	nopt;
	char	*str;
	//char	*trim;
	/*cmd->opt = ft_split(str, ' ');
	if (!cmd->opt)
	{
		free(str);
		return (NULL);
	}*/
	//cmd->opt = cut_empty_str(split_char(str, &nopt, ' '));
	str = rm_space(cmd_str);
	cmd->opt = split_char(str, &nopt, ' ');
	i = 0;
	while (i < nopt)
	{
		if (cmd->opt[i][0] == '\"' || cmd->opt[i][0] == '\'')
		{
			cmd->opt[i] = replace_var(cmd->opt[i], envp);
		}
		i++;
	}
	return (str);
}

int	split_line(t_cmd **lst_cmds, const char *str, int ncmds, char **envp)
{
	t_cmd	*cmd;
	char	*line;

	line = NULL;
	cmd = ft_lstnew(ncmds);
	line = ft_strdup(str);
	if (!line)
		return (1);
	line = get_outfile(cmd, line);
	line = get_infile(cmd, line);
	line = get_command(cmd,line, envp);
	if (!line)
		return (1);
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	cmd->path = ft_split((*envp + 5), ':');
	if (!cmd->path)
		return (1);
	ft_lstadd_back(lst_cmds, cmd);
	free_null(line);
	return (0);
}

void	split_input_cmd(char **cmds, int ncmds, t_cmd *lst_cmds, char **envp)
{
	int	i; 

	if (!check_syntax(cmds))
	{
		i = 0;
		while (cmds[i])
		{
			if (split_line(&lst_cmds, cmds[i], ncmds, envp))
				break ;
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
}



t_cmd	*split_input(char *line, char **envp)
{
	t_cmd	*lst_cmds;
	char	**cmds;
//	int		i;
	int		ncmds;

	lst_cmds = NULL;
	if (!check_quotes(line))
	{
		cmds = split_char(line, &ncmds, '|');
		if (!cmds)
			return (NULL);
		split_input_cmd(cmds, ncmds, lst_cmds, envp);
		/*if (!check_syntax(cmds))
		{
			i = 0;
			while (cmds[i])
			{
				//printf("%s\n", cmds[i]);
				split_line(&lst_cmds, cmds[i], ncmds, envp);
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
		}*/
	}
	else
	{	
		printf("TODO: here exit because of syntax error\n");
	}
        return (lst_cmds);
}
