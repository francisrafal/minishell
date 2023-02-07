#include "minishell.h"

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
	char	*trim;

	str=(char *)malloc(sizeof(char)*(len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, line, len + 1);
	trim =ft_strtrim(str, "\t ");
	free(str);
	return(trim);
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
	int	idx[3];

	len = count_cmds(line);
	//printf("len : %i\n", len);
	init_idx(idx, 3);
	cmds = (char **)malloc(sizeof(char *) * (len + 1));
	while (line[idx[0]] && idx[2] < len)
	{
		if(line[idx[0]] == '"')
                        idx[0]+=get_end_quote(&line[idx[0]+ 1],'"')+1;
                else if(line[idx[0]] == '\'')
                        idx[0]+=get_end_quote(&line[idx[0]+1],'\'')+1;
                else if(line[idx[0]] == '|')
		{
			//printf("start %i and end %i\n", idx[4], idx[0]);
			cmds[idx[2]]=fill_cmds(&line[idx[1]],idx[0]- idx[1]);
			//printf("index %i = %s\n",idx[2], cmds[idx[2]]);
			idx[2]+=1;
			idx[1]=idx[0]+1;
		}
  		idx[0]+=1;
	}
	cmds[idx[2]]= fill_cmds(&line[idx[1]],idx[0]- idx[1]);
	//printf("indesx %i = %s\n",idx[2], cmds[idx[2]]);
	cmds[idx[2] +1]= NULL;
	return(cmds);
}

