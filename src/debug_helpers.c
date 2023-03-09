/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celgert <celgert@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 16:46:14 by celgert           #+#    #+#             */
/*   Updated: 2023/03/09 16:46:17 by celgert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lsof(char *helper_message)
{
	char	cmd1[100];
	char	cmd2[100];

	system("echo '--------------------------' >&2");
	sprintf(cmd1, "echo '%s' >&2", helper_message);
	system(cmd1);
	sprintf(cmd2, "lsof -a -d 0-999 -p %d >&2", getpid());
	system(cmd2);
	system("echo '--------------------------' >&2");
}
