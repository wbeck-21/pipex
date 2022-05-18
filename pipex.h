/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 10:31:31 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/18 16:14:34 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_pipex
{
	pid_t	pid;
	int		*ends;
	int		infile;
	int		outfile;
	int		count_cmds;
	int		id;
	char	**command;
	char	*command_path;
	char	*path;
}	t_pipex;

void	errors(char *err);
void	close_ends(t_pipex *pipex);
void	free_pid(t_pipex *pipex);

#endif