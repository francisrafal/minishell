#include "minishell.h"

int	split_line(t_cmd **lst_cmds, const char *str, int ncmds, t_env *env)
{
	t_cmd	*cmd;
	char	*line;
	t_env_node	*paths;

	line = NULL;
	cmd = ft_lstnew(ncmds);
	line = ft_strdup(str);
	if (!line)
		return (1);
	line = get_outfile(cmd, line);
	line = get_infile(cmd, line);
	line = get_command(cmd,line, env);
	if (!line)
		return (1);
	paths = find_env_node(env, "PATH");
	if (paths == NULL)
		cmd->path = ft_split("", ':');
	else
		cmd->path = ft_split(paths->value, ':');
	if (!cmd->path)
		return (1);
	ft_lstadd_back(lst_cmds, cmd);
	free_null(line);
	return (0);
}

void	split_input_cmd(char **cmds, int ncmds, t_cmd *lst_cmds, t_env *env)
{
	int	i; 

	if (!check_syntax(cmds))
	{
		i = 0;
		while (cmds[i])
		{
			if (split_line(&lst_cmds, cmds[i], ncmds, env))
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



t_cmd	*split_input(char *line, t_env *env)
{
	t_cmd	*lst_cmds;
	char	**cmds;
//	int		i;
	int		ncmds;

	lst_cmds = NULL;
	if (!check_quotes(line) && !check_pipes(line))
	{
		cmds = split_char(line, &ncmds, '|');
		if (!cmds)
			return (NULL);
		split_input_cmd(cmds, ncmds, lst_cmds, env);
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
