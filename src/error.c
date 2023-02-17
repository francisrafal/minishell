#include "minishell.h"

void	perror_exit(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

int     ft_error(char *str, char *err)
{
        int     i;

        if (!str)
                return (2);
        if (err)
        {
		write(2, "minishell: ",11);
                write(2, err, ft_strlen((const char *)err));
                write(2, ": ", 2);
        }
        i = 0;
        while (str[i])
        {
                write(2, &str[i], 1);
                i++;
        }
        write(2, "\n", 1);
        return (2);
}

