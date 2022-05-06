/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 10:31:31 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/06 07:53:05 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	pid_t	child_1;
	pid_t	child_2;
	int		fd[2];
	char	**cmds;
	char	*cmd;
	char	*arg;
}	t_pipex;

#endif