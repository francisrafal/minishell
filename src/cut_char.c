#include "minishell.h"

int	check_str_env(char *str, t_env *env, int k)
{
	t_env_node      *tmp;
	char	*new;
	char	*nb;
	int		size;

	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	if(!ft_strncmp(new, "?", 2))
	{
		free(new);
		nb = ft_itoa(g_exit_code);
		size = (int)ft_strlen(nb);
		free_null(nb);
		return (size);
	}
	tmp = find_env_node(env, new);
	free(new);
	if (tmp == NULL)
	{
		printf("not found\n");
		return (0);
	}
	else
		return ((int)ft_strlen(tmp->value));
}

char	*get_str_env(char *str, t_env *env, int k)
{
	t_env_node	*tmp;
	char    *new;
	
	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	if(!ft_strncmp(new, "?", 2))
	{
		free(new);
		return (ft_itoa(g_exit_code));
	}
	tmp = find_env_node(env, new);
	free(new);
	if (tmp == NULL)
		return (0);
	else
	    return (tmp->value);
}

int	count_chars(char *str, t_env *env)
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
					l =  check_str_env(&str[i+j+1],env ,k);
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
			k = get_next_char(&str[i+1], "\t \"\'\0");
			if (k != 0)
			{
				l =  check_str_env(&str[i+1],env ,k);
                count += k + 1 - l;
			}
			i += k + 1;
		}
		else
			i++;
	}
	return (count);
}

char	*replace_chars(char *str, t_env *env)
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
	size = (int)ft_strlen(str) - count_chars(str, env);
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
					l =  check_str_env(&str[i+1],env ,k);
					if(l)
					{	
						tmp = get_str_env(&str[i+1], env, k);
                        ft_strlcpy(&line[m], tmp, l+1);
						if (str[i+1] == '?' && l == 1)
							free_null(tmp);
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
			k = get_next_char(&str[i+1], "\t \"\'\0");
            if (k != 0)
                        {
				//printf("%s\n %s\n", &str[i+1], line);
				l =  check_str_env(&str[i+1],env ,k);
				if(l)
				{
					tmp = get_str_env(&str[i+1], env, k);
					ft_strlcpy(&line[m], tmp, l+1);
					if (str[i+1] == '?' && l == 1)
							free_null(tmp);
					m += l;
					i += k + 1;
                    // free(tmp);
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
	free_null(str);
	return (line);
}