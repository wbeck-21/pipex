/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 21:42:04 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/12 22:01:55 by wbeck            ###   ########.fr       */
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
		command_path = ft_strjoin(command_path, command);
		free(path);
		if (access(command_path, F_OK) == 0)
			return (command_path);
		(paths)++;
	}
	return (NULL);
}

void	execting(char *argv, char **envp)
{
	char	*command_path;
	char	**command;
	int		i;

	(void) argv;
	i = -1;
	command = ft_split(argv, ' ');
	command_path = find_command_path(command[0], envp);
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

// CHECK DUP2

void	process_child_1(char **argv, char **envp, int *end)
{
	int	fd;

	fd = open(argv[1], O_RDONLY, 0777);
	if (fd < 0)
		return ;
	dup2(end[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(end[0]);
	execting(argv[2], envp);
}

void	process_child_2(char **argv, char **envp, int *end)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0)
		return ;
	dup2(end[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(end[1]);
	execting(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		end[2];
	pid_t	child_1;
	// pid_t	child_2;

	if (argc == 5)
	{
		if (pipe(end) == -1)
			return (1);
		child_1 = fork();
		if (child_1 < 0)
			return (1);
		if (!child_1)
			process_child_1(argv, envp, end);
		waitpid(child_1, NULL, 0);
		// child_2 = fork();
		// if (child_2 < 0)
		// 	return (1);
		// if (!child_2)
		process_child_2(argv, envp, end);
		// waitpid(child_2, NULL, 0);
	}
	return (0);
}
