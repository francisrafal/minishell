#include "minishell.h"

int	open_close_outfile(char * tmp, char *file, t_cmd *cmd)
{
	int	fd_out;

		if(tmp[1] == '>' )
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
		{
                        close(fd_out);
			return (0);
		}
		else
			return (-1);
}


char	*get_outfile(t_cmd *cmd, char *line)
{
        int     fd_out;
        char    *tmp;
        char    *file;

        file = NULL;
        tmp = ft_strchr(line, '>');
        if (!tmp)
	{
		printf("no >\n");
                return (line);
	}
        while (tmp)
        {
                file = get_file_name(line, '>');
		printf("file: |%s|\n", file);
                if(open_close_outfile(tmp, file, cmd))
                {
			ft_printf("error");
                }
		line = cut_word(line, '>');
                tmp = ft_strchr(line, '>');
                if (tmp)
			free(file);
        }
        printf("cutted line : |%s|\n", line);
        printf("output file: |%s|\n", file);
        fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        cmd->fd_out = fd_out;
        free(file);
        return (line);
}
