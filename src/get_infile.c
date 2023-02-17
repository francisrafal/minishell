#include "minishell.h"

/*int	here_doc(char *av)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
		return (ft_error(strerror(errno), ".heredoc_tmp"));
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf) < 0)
			exit(1);
		if (!ft_strncmp(av, buf, ft_strlen(av)))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	file = open(".heredoc_tmp", O_RDONLY, 0644);
	return (file);
}

int	get_infile(t_cmd *cmd, char *line)
{
	int	i;

	i = 0;


	if (ft_strncmp("here_doc", av[1], 9))
	{
		fd_in = open(av[1], O_RDONLY);
		if (fd_in < 0)
			ft_error(strerror(errno), av[1]);
		return (fd_in);
	}
	else
	{
		fd_in = here_doc(av[2]);
		if (fd_in < 0)
			ft_error(strerror(errno), av[1]);
		return (fd_in);
	}
}*/
/*
int     get_outfile(t_cmd *cmd, char *line)
{
        int     fd_out;
        char    *tmp;
        (void)cmd;
        char    *file;

        printf("get output file");
        return(1);
        tmp = ft_strchr(line, '>');
        while (tmp)
        {
                file = get_file_name(tmp);
                if (tmp[1] == '>')
                {
                        cmd->re_out_app = 1;
                        fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else
                        fd_out = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
                //free(file)
                if (!fd_out)
                        return (1);

                cmd->fd_out = fd_out;
        }
        return (0);
}*/

/*char	*cut_line(char *line, char c)
{
	int		i;
	int		j;
	int		k;
	char	*new;

	i = get_next_char(line, &c);
	j = 0;
	while (line[i + j] && (line[i + j] == '\t'  || line[i + j] == ' ' || line[i + j] == c))
                j++;
	j += get_next_char(&line[i + j], "\t ");
	new = (char *)malloc(sizeof(char) * ((int)ft_strlen(line) - j));
	k = 0;
	while (k < (int)ft_strlen(line))
	{
		if (k < i)
			new[k] = line[k];
		else
			new[k] = line[k + i + j];
		k++;
	}
	free(line);
	return (new);
}*/

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
