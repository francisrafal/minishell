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

char    *cut_word(char *str, int start, int end)
{
        int     size;
        char    *new;

        size = (int)ft_strlen(str) - end  + start -1;
        //printf("new length line: %i = %i - %i + %i\n", size, (int)ft_strlen(str),end, start);
        new = (char *)malloc(sizeof(char) * (size + 1));
        if (!new)
                return (NULL);
        //printf("cut word: %i bis %i\n", start + 1, size-start);
        ft_strlcpy(new, str, start);
        if (size - start != 0)
                ft_strlcpy(&new[start], &str[end + 1], size - start);
        //printf("new line after cut: |%s|\n", new);
        free(str);
        return(new);
}

char	*get_file_name(char *str, char **file, char c)
{
	int	i;
	int	j;
	int	k;
	
	k = get_next_char(str, ">");
	i = k + 1;
	if (str[i] && str[i] == c)
                i++;
	while (str[i] && (str[i] == '\t'  || str[i] == ' '))
		i++;
	j = get_next_char(&str[i], "\t ");
	printf("next >: %i: %i :%i\n", i,j,i+j-1);
	*file = (char *)malloc(sizeof(char) * (j + 1));
	if (!*file)
		return (NULL);
	ft_strlcpy(*file, &str[i], j+1);
	str = cut_word(str, k, i+j-1);
	return(str);

}

int	get_outfile(t_cmd *cmd, char *line)
{
	int	fd_out;
	char 	*tmp;
//	(void)cmd;
	char	*file; 
	int	app;
	
	app =0;
	file = NULL;
	tmp = ft_strchr(line, '>');
	if (!tmp)
		return (0);
	while (tmp)
	{
		if(tmp[1] == '>' )
			app =1;
		line = get_file_name(line, &file, '>');
		if (app)
		{	
			cmd->re_out_app = 1;
			cmd->re_out = 0;
			fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else{
			cmd->re_out_app = 0;
			cmd->re_out = 1;
			fd_out = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
		}
		if (fd_out)
			close(fd_out);
		tmp = ft_strchr(line, '>');
		if (tmp)
			free(file);
	}
	printf("cutted line : |%s|\n", line);
	printf("output file: |%s|\n", file);
	fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	cmd->fd_out = fd_out;
	free(file);
	return (0);
}
