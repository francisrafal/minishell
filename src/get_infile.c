#include "minishell.h"

int	here_doc(char *av)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
		return (ft_error(strerror(errno), ".heredoc_tmp"));
	while (1)
	{
		//write(1, "heredoc> ", 9);
		//if (get_next_line(0, &buf) < 0)
		//	exit(1);
		buf = readline(HERE_DOC);
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

/*int	get_infile(t_cmd *cmd, char *line)
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

int	handle_infile(char * tmp, char *file, t_cmd *cmd)
{
	int	fd_in;
	//char	*tmp;

		if(tmp[1] == '<' )
                {
			printf("here_doc fun");
                        cmd->read_in = 1;
                        cmd->re_in = 0;
                        fd_in = here_doc(file);
                }
                else{
                        cmd->read_in = 0;
                        cmd->re_in = 1;
			fd_in = open(file, O_RDONLY);
			if (fd_in < 0)
				ft_error(strerror(errno), file);
                }
                /*if (fd_in >= 0)
			close(fd_in);*/
		return (fd_in);
}

char	*get_infile(t_cmd *cmd, char *line)
{
        int     fd_in;
        char    *tmp;
        char    *file;

        file = NULL;
        tmp = ft_strchr(line, '<');
        if (!tmp)
	{
		printf("no input redirection \n");
                return (line);
	}
        while (tmp)
        {
                file = get_file_name(line, '<');
		printf("infile: |%s|\n", file);
		fd_in = handle_infile(tmp, file, cmd);
		line = cut_word(line, '<');
                tmp = ft_strchr(line, '<');
                if (tmp)
			free(file);
        }
        printf("cutted + timmed line : |%s|\n", line);
        printf("input file: |%s|\n", file);
	//fd_in = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        cmd->fd_in = fd_in;
        free(file);
        return (line);
}
