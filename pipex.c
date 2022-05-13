/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 21:42:04 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/13 20:28:44 by wbeck            ###   ########.fr       */
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

void	execting(char *argv, char **envp)
{
	char	*command_path;
	char	**command;
	int		i;

	i = -1;
	command = ft_split(argv, ' ');
	command_path = find_command_path(command[0], envp);
	printf("%s", command_path);
	if (!command_path)
	{
		while (command[i])
		{
			free(command[i]);
			i++;
		}
		free(command);
		return ;
	}
	execve(command_path, command, envp);
}

void	process_child(char **argv, char **envp, int *end)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return ;
	dup2(end[1], 1);
	close(end[0]);
	dup2(fd, 0);
	execting(argv[2], envp);
}

void	process_parent(char **argv, char **envp, int *end)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0000664);
	if (fd < 0)
		return ;
	dup2(end[0], 0);
	close(end[1]);
	dup2(fd, 1);
	execting(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		end[2];
	pid_t	pid;

	if (argc == 5)
	{
		if (pipe(end) == -1)
			return (1);
		pid = fork();
		if (pid < 0)
			return (1);
		if (!pid)
			process_child(argv, envp, end);
		process_parent(argv, envp, end);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
