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

char	*get_command(t_cmd *cmd, char *cmd_str, t_env *env)
{
	int	i;
	int	nopt;
	char	*str;
	char	*tmp;

//	str = prun_str(cmd_str);
	tmp = replace_chars(cmd_str, env);
	str = prun_str(tmp);
	cmd->opt = split_char(str, &nopt, ' ');
	if (!cmd->opt)
		return (NULL);
	i = 0;
	while (i < nopt)
	{
		//if (is_char(cmd->opt[i], '\'') || is_char(cmd->opt[i], '"'))
		//{
		//cmd->opt[i] = replace_chars(cmd->opt[i], env);
		//}
		i++;
	}
	return (str);
}
