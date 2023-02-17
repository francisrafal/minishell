#include "minishell.h"

int     get_next_char(char *line, char *cset)
{
        int     i;

        i = 0;
        while (line[i])
        {
                if (ft_strchr(cset, line[i]))
                        return (i);
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
			return (i);
                i++;
        }
        return (i);
}

char    *cut_word(char *str,  char c)
{
        int     i;
        int     j;
        int     k;
	int 	size;
	char	*new;

        k = get_char(str, '>');
        i = k + 1;
        if (str[i] && str[i] == c)
                i++;
        while (str[i] && (str[i] == '\t'  || str[i] == ' '))
                i++;
        j = get_next_char(&str[i], "\t ");
        size = (int)ft_strlen(str) - i-j + k;
        new = (char *)malloc(sizeof(char) * (size + 1));
        if (!new)
                return (NULL);
        ft_strlcpy(new, str, k+1);
       	ft_strlcpy(&new[k], &str[i + j], size - k +1);
        free(str);
        return(new);
}


char	*get_file_name(char *str, char c)
{
	int	i;
	int	j;
	int	k;
	char 	*file;

	k = get_char(str, '>');
	i = k + 1;
	if (str[i] && str[i] == c)
                i++;
	while (str[i] && (str[i] == '\t'  || str[i] == ' '))
		i++;
	j = get_next_char(&str[i], "\t ");
	//printf("next >: %i: %i :%i\n", i,j,i+j-1);
	file = (char *)malloc(sizeof(char) * (j + 1));
	if (!file)
		return (NULL);
	ft_strlcpy(file, &str[i], j+1);
	return(file);
}
