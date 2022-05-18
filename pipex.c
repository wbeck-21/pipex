/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 15:48:39 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/18 16:19:03 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	process_pid(t_pipex *pipex, char **argv, char **enpv, int i)
{
	pipex->pid = fork();
	if (!pipex->pid)
	{
		if (i == 0)
		{
			dup2(pipex->infile, 0);
			dup2(pipex->ends[1], 1);
		}
		else if (i == pipex->count_cmds - 1)
		{
			dup2(pipex->ends[2 * i - 2], 0);
			dup2(pipex->outfile, 1);
		}
		else
		{
			dup2(pipex->ends[2 * i - 2], 0);
			dup2(pipex->ends[2 * i + 1], 1);
		}
		close_ends(pipex);
		pipex->command = ft_split(argv[2 + i], ' ');
		find_command_path(pipex, pipex->command[0], enpv);
		if (!(pipex->command_path))
			free_pid(pipex);
		execve(pipex->command_path, pipex->command, enpv);
	}
}

void	create_pid(t_pipex pipex, char **argv, char **envp)
{
	int	i;

	i = -1;
	while (pipex.count_cmds > ++i)
		process_pid(&pipex, argv, envp, i);
	close_ends(&pipex);
	waitpid(-1, NULL, 0);
	close(pipex.infile);
	close(pipex.outfile);
	free(pipex.ends);
}

void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
		errors("Open failed on input file");
	pipex->outfile = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0000644);
	if (pipex->outfile < 0)
		errors("Open failed on output file");
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	if (argc == 5)
	{
		open_files(&pipex, argv);
		pipex.count_cmds = argc - 3;
		pipex.ends = (int *)malloc(sizeof(int) * (pipex.count_cmds - 1) * 2);
		if (!pipex.ends)
			errors("Out of memory");
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
		create_pid(pipex, argv, envp);
	}
	else
		errors("Error");
	return (0);
}
