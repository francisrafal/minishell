#include "minishell.h"

char	*replace_vars(char *cmd, char **envp)
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


/*char	*cutted_spaces(char *str, char *new, char c)
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
}*/
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
}

char	*get_command(t_cmd *cmd, char *cmd_str, t_env *env)
{
	int	i;
	int	nopt;
	char	*str;

	str = prun_str(cmd_str);
	cmd->opt = split_char(str, &nopt, ' ');
	if (!cmd->opt)
		return (NULL);
	i = 0;
	while (i < nopt)
	{
		if (is_char(cmd->opt[i], '\'') || is_char(cmd->opt[i], '"'))
		{
			cmd->opt[i] = replace_chars(cmd->opt[i], env);
		}
		i++;
	}
	return (str);
}
