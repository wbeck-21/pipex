/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 10:31:31 by wbeck             #+#    #+#             */
/*   Updated: 2022/06/14 23:29:14 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include "../bonus/get_next_line/get_next_line.h"
# include "../libft/libft.h"

typedef struct s_pipex
{
	pid_t	pid;
	int		*ends;
	int		infile;
	int		outfile;
	int		here_doc;
	int		count_cmds;
	int		id;
	char	**command;
	char	*command_path;
	char	*path;
}	t_pipex;

void	process_here_doc(t_pipex *pipex, char *argv);
void	open_files(t_pipex *pipex, int argc, char **argv);
void	find_command_path(t_pipex *pipex, char *command, char **envp);
void	errors(char *err);
void	close_ends(t_pipex *pipex);
void	free_pid(t_pipex *pipex);

#endif