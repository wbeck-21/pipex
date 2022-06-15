/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 16:09:58 by wbeck             #+#    #+#             */
/*   Updated: 2022/06/14 23:26:12 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	if (!(ft_strncmp("here_doc", argv[1], 9)))
	{
		process_here_doc(pipex, argv[2]);
		pipex->outfile = open(argv[argc - 1],
				O_APPEND | O_TRUNC | O_WRONLY, 0000644);
		pipex->here_doc = 1;
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
			errors("Open failed on input file");
		pipex->outfile = open(argv[argc - 1],
				O_CREAT | O_TRUNC | O_WRONLY, 0000644);
	}
	if (pipex->outfile < 0)
		errors("Open failed on output file");
}

void	find_command_path(t_pipex *pipex, char *command, char **envp)
{
	char	**paths;
	int		i;

	while (!(ft_strnstr(*envp, "PATH=", 5)))
		envp++;
	paths = ft_split(*envp + 5, ':');
	i = -1;
	while (paths[++i])
	{
		pipex->path = ft_strjoin(paths[i], "/");
		pipex->command_path = ft_strjoin(pipex->path, command);
		free(pipex->path);
		if (!access(pipex->command_path, X_OK))
			return ;
		free(pipex->command_path);
	}
	pipex->command_path = NULL;
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

void	errors(char *err)
{
	perror(err);
	exit(1);
}

void	close_ends(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < ((pipex->count_cmds - pipex->here_doc - 1) * 2))
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
