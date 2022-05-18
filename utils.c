/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 16:09:58 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/18 16:10:20 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	errors(char *err)
{
	perror(err);
	exit(1);
}

void	close_ends(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < ((pipex->count_cmds - 1) * 2))
		close(pipex->ends[i]);
}

void	free_pid(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (pipex->command[++i])
		free(pipex->command[i]);
	free(pipex->command);
	free(pipex->command_path);
	exit(1);
}
