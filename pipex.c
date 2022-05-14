/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 15:48:39 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/14 19:48:43 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	*find_command_path(char *command, char **envp)
{
	char	*command_path;
	char	**paths;
	char	*path;

	while (!(ft_strnstr(*envp, "PATH=", 5)))
		envp++;
	paths = ft_split(*envp + 5, ':');
	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		command_path = ft_strjoin(path, command);
		if (!access(command_path, X_OK))
			return (command_path);
		free(path);
		free(command_path);
		paths++;
	}
	return (0);
}

void	close_ends(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < ((pipex->count_cmds - 1) * 2))
		close(pipex->ends[i]);
}

void	process_pid(t_pipex pipex, char **argv, char **enpv, int i)
{
	pipex.pid = fork();
	if (!pipex.pid)
	{
		if (i == 0)
		{
			dup2(pipex.infile, 0);
			dup2(pipex.ends[1], 1);
		}
		else if (i == pipex.count_cmds - 1)
		{
			dup2(pipex.ends[2 * i - 2], 0);
			dup2(pipex.outfile, 1);
		}
		else
		{
			dup2(pipex.ends[2 * i - 2], 0);
			dup2(pipex.ends[2 * i + 1], 1);
		}
		close_ends(&pipex);
		pipex.command = ft_split(argv[2 + i], ' ');
		pipex.command_path = find_command_path(pipex.command[0], enpv);
		execve(pipex.command_path, pipex.command, enpv);
	}
}

int	main(int argc, char **argv, char **enpv)
{
	t_pipex	pipex;
	int		i;

	if (argc == 5)
	{
		pipex.infile = open(argv[1], O_RDONLY);
		pipex.outfile = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0000644);
		pipex.count_cmds = argc - 3;
		pipex.ends = (int *)malloc(sizeof(int) * (pipex.count_cmds - 1) * 2);
		i = -1;
		while (pipex.count_cmds - 1 > ++i)
		{
			if (pipe(pipex.ends + 2 * i) < 0)
			{
				close(pipex.infile);
				close(pipex.outfile);
				free(pipex.ends);
			}
		}
		i = -1;
		while (pipex.count_cmds > ++i)
			process_pid(pipex, argv, enpv, i);
		close_ends(&pipex);
		waitpid(-1, NULL, 0);
		close(pipex.infile);
		close(pipex.outfile);
		free(pipex.ends);
	}
	return (0);
}
