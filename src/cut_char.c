//#include "minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] && (s[i] != (char)c))
	{
		i++;
	}
	if ((char)c == 0)
		return ((char *)s + i);
	if (!s[i])
		return (NULL);
	return ((char *)s + i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*c1;
	unsigned char	*c2;

	if (n == 0)
		return (0);
	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	i = 0;
	if (c1[i] == '\0')
		return (c1[i] - c2[i]);
	while (c1[i] != '\0' && i < n)
	{
		if (c1[i] != c2[i])
			return (c1[i] - c2[i]);
		i++;
	}
	if (c1[i] == '\0' && i != n)
		return (c1[i] - c2[i]);
	return (0);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	l;
	size_t	i;

	l = ft_strlen(src);
	if (size == 0)
		return (l);
	i = 0;
	while ((src[i] != '\0') && (i < size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (l);
}
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	s1_len;
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	new_str = (char *)malloc((s1_len + ft_strlen(s2) + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		new_str[s1_len + i] = s2[i];
		i++;
	}
	new_str[s1_len + i] = '\0';
	return (new_str);
}

int     get_next_char(char *line, char *cset)
{
        int     i;

        i = 0;
        while (line[i])
        {
                if (ft_strchr(cset, line[i]))
                {
                        if (i>0 && line[i-1] == '\\' && line[i] != '\t' && line[i] != ' ')
			{
				i++;
				continue;
			}
                        return (i);
                }
                i++;
        }
        return (i);
}

int	get_end_quote(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == c)
		{
			if (i == 0)
				return (i);
			else if(i > 0  && line[i - 1] != '\\')
				return (i);
		}
		i++;
	}
	return (i);
}

int     get_char(char *line, char c)
{
        int     i;

        i = 0;
        while (line[i])
        {
                if (line[i] == c)
                {
                        if (i>0 && line[i-1] == '\\' && line[i] != '\t' && line[i] != ' ')
			{
				i++;
				continue;
			}
                        return (i);
                }
                i++;
        }
        return (i);
}

int	check_str_env(char *str, char **envp, int k)
{
	char *new;
	char *tmp;


	new = (char *)malloc(sizeof(char) * (k + 1));
	if (!new)
		return (0);
	ft_strlcpy(new, str, k + 1);
	printf("%s\n", new);
	tmp = ft_strjoin(new, "=");
	free(new);
	if (!tmp)
		return (0);
	while (ft_strncmp(tmp, *envp, k+1))
		envp++;
	if (!(*envp))
		return (0);
	free(tmp);
	printf("%s\n", *envp + k + 1);
	return ((int)ft_strlen(*envp + k + 1));
}

char	*get_str_env(char *str, char **envp, int k)
{
        char *new;
        char *tmp;


        new = (char *)malloc(sizeof(char) * (k + 1));
        if (!new)
                return (0);
        ft_strlcpy(new, str, k + 1);
        tmp = ft_strjoin(new, "=");
        free(new);
        if (!tmp)
                return (0);
        while (ft_strncmp(tmp, *envp, k+1))
                envp++;
        if (!(*envp))
                return (0);
	free(tmp);
	new = (char *)malloc(sizeof(char) * ((int)ft_strlen(*envp + k + 1) + 1));
        if (!new)
                return (0);
	ft_strlcpy(new, *envp + k + 1,(int)ft_strlen(*envp + k + 1) + 1);
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
			if (j < get_end_quote(&str[i + 1], '"'))
			{
				k = get_next_char(&str[i+j+1], "\t \"\'");
				if (k != 0)
				{
					l =  check_str_env(&str[i+j+1],envp ,k);
					if (l)
						count += k + 1 - l;
				}
			//	j += get_next_char(&str[i + j + k + 1], "$") + 1;
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

/*char	*replace_chars(char *str, char **envp)
{
	char 	*line;
	int	size;
        int     i;
	int	m;
        int     j;
        int     k;
        int     l;
        int     count;

	size = (int)ft_strlen(str) - count_chars(str, envp);
	line = (char *)malloc(sizeof(char) *(size +1));
	if (!line)
		return (NULL);
        count = 0;
        i = 0;
	m = 0;
        while (m < size)
        {
                if (str[i] == '"')
                {
			j = get_next_char(&str[i + 1], "$") + 1;
                        if (j < get_end_quote(&str[i + 1], '"'))
                        {
                                k = get_next_char(&str[i+j+1], "\t \"\'");
                                if (k != 0)
                                {
                                        l =  check_str_env(&str[i+j+1],envp ,k);
                                        if (l)
					{
                                                 ft_strlcpy(&line[m], get_str_env(str, envp,k), l+1);
						 m += l;
					}
                                }
                        }
			ft_strlcpy(&line[m], &str[i+k+1], get_end_quote(&str[i + 1], '"'));
			m += get_end_quote(&str[i + 1], '"') + 1;
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
}*/


/*{
	int i;
	int j;

	i = get_char(str, c);
	if (str[i] == '\0')
		return (0);
	j = 1;
	i++;
	while (str[i])
	{
		i += get_char(&str[i], c);
		if (str[i] != '\0')
		{
			j++;
			i++;
		}
	}
	return	(j);
}*/

/*
char    *cut_word2(char *str,  char c)
{
        int     i;
        int     j;
        int     k;
	int 	size;
	char	*new;
	char	*trim;

        k = get_char(str, c);
        i = k + 1;
        j = get_next_char(&str[i], "\t \"\'");
        size = (int)ft_strlen(str) - i-j + k;
        new = (char *)malloc(sizeof(char) * (size + 1));
        if (!new)
                return (NULL);
        ft_strlcpy(new, str, k+1);
       	ft_strlcpy(&new[k], &str[i + j], size - k +1);
        free(str);
	trim =  ft_strtrim(new, "\t ");
        if (!trim)
                return (NULL);        
	free(new);
        return(trim);
}


char	*get_file_name(char *str, char c)
{
	int	i;
	int	j;
	int	k;
	char 	*file;
	char	*trim;

	k = get_char(str, c);
	i = k + 1;
	if (str[i] && str[i] == c)
                i++;
	while (str[i] && (str[i] == '\t'  || str[i] == ' '))
		i++;
	j = get_next_char(&str[i], "\t ><");
	//printf("next >: %i: %i :%i\n", i,j,i+j-1);
	file = (char *)malloc(sizeof(char) * (j + 1));
	if (!file)
		return (NULL);
	ft_strlcpy(file, &str[i], j+1);
	trim =  ft_strtrim(file, "\"\'");
        if (!trim)
                return(NULL);
	free(file);
	return (trim);
}*/






int	main()
{
	int	i;
	char str[]= "he\"$USER1 und $USER , was\" geht $jh nkj";
	char str1[]= "\"hello  was geht $ jh\" nkj";
	char str2[]= "hello was \n geht jh kj";
	char *env[3];

	env[0]="USER=celgert";
	env[1]="USER1=francis";
	env[3]=NULL;
	i = 0; 
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			str[i] = 'm';
		i++;
	}

	i = 0;
        while (str1[i])
        {
                if (str[i] == '\\')
                        str[i] = 'm';
                i++;
        }

	printf("%s\n", str);
	printf("%i\n", count_chars(str,env));
	printf("\n");

	printf("%s\n", str1);
	printf("%i\n", count_chars(str1,env));
	printf("\n");


	printf("%s\n", str2);
	printf("%i\n", count_chars(str2, env));

	return (0);
}
