#include "minishell.h"

int	check_str_env(char *str, t_env env, int k)
{
	char	*new;
	char	*tmp;
	int		i;

	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	tmp = ft_strjoin(new, "=");
	free(new);
	if (!tmp)
		return (0);
	i = 0;
	// CORRECT ENVP HERE:
	while (envp[i] && ft_strncmp(tmp, envp[i], k+1))
		i++;
	if (!(envp[i]))
		return (0);
	free(tmp);
	return ((int)ft_strlen(&envp[i][k + 1]));
}

char	*get_str_env(char *str, char **envp, int k)
{
	char	*new;
    char	*tmp;
	int		i;

	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	tmp = ft_strjoin(new, "=");
	free(new);
	if (!tmp)
		return (0);
	i = 0;
	while (envp[i] && ft_strncmp(tmp, envp[i], k+1))
		i++;
	if (!(envp[i]))
		return (0);
	free(tmp);
	new = (char *)malloc(sizeof(char) * ((int)ft_strlen(&envp[i][k + 1]) + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, &envp[i][k + 1], (int)ft_strlen(&envp[i][k + 1]) + 1);
	return(new);
}
int	count_chars(char *str, char **envp)
{
	int	i;
	int	j;
	int	k;
	int	l;
	int	count;

	count = 0;
	i = 0;
	while (str[i] && i < (int)ft_strlen(str))
	{
		if (str[i] == '"')
		{
			count += 2;
			j = get_next_char(&str[i + 1], "$") + 1;
			while (j < get_end_quote(&str[i + 1], '"'))
			{
				k = get_next_char(&str[i+j+1], "\t \"\'");
				if (k != 0)
				{
					l =  check_str_env(&str[i+j+1],envp ,k);
					count += k + 1 - l;
				}
				j += get_next_char(&str[i + j + k + 1], "$")+ k + 1;
			}
			i += get_end_quote(&str[i + 1], '"') + 2;
		}
		else if (str[i] == '\'')
		{
			count += 2;
			i += get_end_quote(&str[i + 1], '\'') + 22;
		}
		else if (str[i] == '$')
		{
			k = get_next_char(&str[i+1], "\t \"\'");
                        if (k != 0)
                        {
				l =  check_str_env(&str[i+j+1],envp ,k);
                                count += k + 1 - l;
			}
			i += k + 1;
		}
		else
			i++;
	}
	return (count);
}

char	*replace_chars(char *str, char **envp)
{
	char 	*line;
	int	size;
	int	i;
	int	m;
	int	j;
	int	k;
	int	l;
 //   int     i[5];
	char 	*tmp;
	
	//init_idx(i,5);
	size = (int)ft_strlen(str) - count_chars(str, envp);
	line = (char *)malloc(sizeof(char) *(size +1));
	if (!line)
		return (NULL);
	

	m = 0;
	i = 0;
	while (str[i] && i < (int)ft_strlen(str))
	{
		if (str[i] == '"')
		{
			j = get_next_char(&str[i + 1], "$") + 1;
			while (j < get_end_quote(&str[i + 1], '"'))
			{
				ft_strlcpy(&line[m], &str[i+1], j);
				//printf("%s\n %s\n", &str[i], line);
				i += j;
				m += j - 1;
				//printf("%s\n %s\n", &str[i], line);
				k = get_next_char(&str[i+1], "\t \"\'");
				if (k != 0)
				{
					//printf("%s\n %s\n", &str[i], line);
					l =  check_str_env(&str[i+1],envp ,k);
					if(l)
					{	
						tmp = get_str_env(&str[i+1], envp, k);
                                                ft_strlcpy(&line[m], tmp, l+1);
                                               	m += l;
                                                i += k;
						//printf("%s\n %s\n", &str[i], line);
					}
					else
						i += k; 
				}
				j += get_next_char(&str[i + 1], "$");
			}
			ft_strlcpy(&line[m], &str[i+1], get_end_quote(&str[i + 1], '"') + 1);
                        m += get_end_quote(&str[i + 1], '"');
                        i += get_end_quote(&str[i + 1], '"')+2;

		}
		else if (str[i] == '\'')
		{
			l = get_end_quote(&str[i + 1], '\'');
			ft_strlcpy(&line[m], &str[i + 1], l + 1);
			m += l;
			i += l + 2;
		}
		else if (str[i] == '$')
		{
			k = get_next_char(&str[i+1], "\t \"\'");
                        if (k != 0)
                        {
				//printf("%s\n %s\n", &str[i+1], line);
				l =  check_str_env(&str[i+1],envp ,k);
				if(l)
				{
					tmp = get_str_env(&str[i+1], envp, k);
					ft_strlcpy(&line[m], tmp, l+1);
					m += l;
					i += k + 1;
                                        free(tmp);
				}
				else
					i += k + 1;
			}
		}
		else
		{
			ft_strlcpy(&line[m], &str[i], 2);
			i++;
			m++;
		}
	}
	return (line);
}

/*int	count_chars(char *str, char **envp)
{
	int	i;
	int	j;
	int	k;
	int	l;
	int	count;

	count = 0;
	i = 0;
	while (str[i] && i < (int)ft_strlen(str))
	{
		if (str[i] == '"')
		{
			count += 2;
			j = get_next_char(&str[i + 1], "$") + 1;
			while (j < get_end_quote(&str[i + 1], '"'))
			{
				k = get_next_char(&str[i+j+1], "\t \"\'");
			//	printf("j: %i ; k: %i\n",j,k);
				if (k != 0)
				{
					l =  check_str_env(&str[i+j+1],envp ,k);
					//printf("l: %i\n", l);
					if (l)
						count += k + 1 - l;
				}
				j += get_next_char(&str[i + j + k + 1], "$")+ k + 1;
			}
			i += get_end_quote(&str[i + 1], '"') + 1;

		}
		else if (str[i] == '\'')
		{
			count += 2;
			i += get_end_quote(&str[i + 1], '\'') + 1;
		}
		if (str[i] && i != (int)ft_strlen(str))
			i++;
	}
	return (count);
}

char	*replace_chars(char *str, char **envp)
{
	char 	*line;
	int	size;
        int     i[5];
	char 	*tmp;
	
	init_idx(i,5);
	size = (int)ft_strlen(str) - count_chars(str, envp);
	line = (char *)malloc(sizeof(char) *(size +1));
	if (!line)
		return (NULL);

        i[0] = 0;
        while (i[1] < size)
        {
			if (str[i[0]] == '"')
			{
				i[2] = get_next_char(&str[i[0] + 1], "$") + 1;
				//printf("i[0] : %i , i[1] : %i, i[2]: %i , %i\n ", i[0], i[1], i[2], get_end_quote(&str[i[0] + 1], '"'));
				while (i[2] < get_end_quote(&str[i[0] + 1], '"'))
				{
					ft_strlcpy(&line[i[1]], &str[i[0]+1], i[2]);
					i[1] += i[2] -1;
					i[0] += i[2] -1;
					//printf("%i and %i\n", i[0],i[1]);
					i[3] = get_next_char(&str[i[0]+i[2]+1], "\t \"\'");
					if (i[3] != 0)
					{
						i[4] =  check_str_env(&str[i[0]+i[2]+1],envp, i[3]);
						if (i[4])
						{
							tmp = get_str_env(&str[i[0]+i[2]+1], envp, i[3]);
							if (!tmp)
								return (NULL);
							ft_strlcpy(&line[i[1]], tmp, i[4]+1);
							free(tmp);
							i[1] += i[4];
							i[0] += i[3] + 1;
							//printf("i[0] : %i ,i[1]: %i, %c\n ", i[0], i[1], str[i[0]]);
							//return (line);
						}
					}
					i[2] += get_next_char(&str[i[0]], "$")-1;
					//ft_strlcpy(&line[i[1]], &str[i[0]], i[2]+1);
					//printf("i : %i ,m: %i, %c\n ", i[0], i[0] +i[2], str[i[0]]);
					//return(line);
				}
				ft_strlcpy(&line[i[1]], &str[i[0]+1], get_end_quote(&str[i[0] + 1], '"') + 1);
				i[1] += get_end_quote(&str[i[0] + 1], '"');
				i[0] += get_end_quote(&str[i[0] + 1], '"')+1;
				//printf("i[0] : %i ,i[1]: %i\n ", i[0], i[1]);
			}
			else if (str[i[0]] == '\'')
			{
				i[3] = get_end_quote(&str[i[0] + 1], '\'');
				ft_strlcpy(&line[i[1]], &str[i[0] + 1], i[3] + 1);
				i[1] += i[3];
				i[0] += i[3] + 2;
			}
			else if (str[i[0]] && i[0] != (int)ft_strlen(str))
			{
				//printf("i[0] : %i ,i[1]: %i\n ", i[0], i[1]);
				ft_strlcpy(&line[i[1]], &str[i[0]], 2);
				i[0]++;
				i[1]++;
			}
			else 
				i[1]++;
		}
    return (line);
}*/

