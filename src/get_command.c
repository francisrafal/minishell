#include "minishell.h"

/*char	*replace_vars(char *cmd, char **envp)
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

int	is_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if(str[i] == c)
		{
			if (i > 0 && str[i-1] == '\\')
			{
				i++;
				continue ;
			}
			return (1);
		}
		i++;
	}
	return (0);
}*/
int	count_empty_opt(char **cmds)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while(cmds[i])
	{
		if (!ft_strncmp(cmds[i], "", 1))
			count++;
		i++;
	}
	return (count);
}

char	**cut_empty_opt(char **cmds, int nopt)
{
	int		size;
	char	**tmp;
	int		i;
	int		j;

	size = nopt - count_empty_opt(cmds);
	if (count_empty_opt(cmds) == 0)
		return (cmds);
	if(size == 0)
	{
		tmp = (char **)malloc(sizeof(char *) * 2);
		if(!tmp)
			return (NULL);
		tmp[0] = (char *)malloc(sizeof(char));
		tmp[0] = "";
		tmp[1] = NULL;
		cmds = free_arr_null(cmds);
		return (tmp);
	}
	i = 0;
	j = 0;
	tmp = (char **)malloc(sizeof(char *) * (size + 1));
	while (i < size)
	{
		if (!ft_strncmp(cmds[j], "", 1))
			j++;
		tmp[i] = ft_strdup(cmds[j]);
		i++;
		j++;
	}
	tmp[i] = NULL;
	cmds = free_arr_null(cmds);
	return (tmp);	
}

char	*get_command(t_cmd *cmd, char *cmd_str, t_env *env)
{
	int		nopt;
	int		i;
	char	*str;
	//char	*tmp;

	//tmp = replace_chars(cmd_str, env);
	str = prun_str(cmd_str);
	cmd->opt = split_char(str, &nopt, ' ');
	if (!cmd->opt)
		return (NULL);
	i = 0;
	while (cmd->opt[i])
	{
		cmd->opt[i] = replace_chars(cmd->opt[i],env);
		if (!cmd->opt[i])
			return (NULL);	
		i++;
	}
	cmd->opt = cut_empty_opt(cmd->opt, nopt);
	return (str);
}