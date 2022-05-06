/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 21:42:04 by wbeck             #+#    #+#             */
/*   Updated: 2022/05/06 23:08:52 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// write processor of error, validation?? makefile, free? 

void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	pipex->outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex->infile == -1 || pipex->outfile == -1)
		return ;
}

void	find_cmds(char **envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	pipex.cmds = ft_split(envp + 5, ':');
	if (pipex.cmds == NULL)
		return ;
}

void	find_cmd(char **cmds, char *arg)
{
	while (*cmds)
	{
		pipex.cmd = ft_strjoin(ft_strjoin(*cmds, "/"), arg);
		cmds++;
	}	
}

void	child_proc_1(t_pipex pipex, char **argv, char **envp)
{
	dup2(pipex.fd[1], 1);
	dup2(pipex.infile, 0);
	pipex.arg = ft_split(argv[1], ' ');
	find_cmd(pipex.cmds, pipex.arg[0]);
	execve(pipex.cmd, pipex.cmd_arg, envp);
}

void	child_proc_2(t_pipex pipex, char **argv, char **envp)
{
	dup2(pipex.fd[0], 0);
	dup2(pipex.outfile, 1);
	pipex.arg = ft_split(argv[3], ' ');
	find_cmd(pipex.cmds, pipex.arg[0]);
	execve(pipex.cmd, pipex.cmd_arg, envp);
}

#include <stdio.h>

int	main(int agrc, char **argv, char **envp)
{
	t_pipex	pipex; // malloc?


	if (agrc == 5)
	{
		open_files(&pipex, argv);
		if (pipe(pipex.fd) == -1)
			return (0);
		find_cmds(envp);
		pipex.child_1 = fork();
		if (pipex.child_1 < 0)
        	return (perror("Fork: "));
		if(!pipex.child_1)
			child_proc_1(pipex, argv, envp);
		pipex.child_2 = fork();
		if (pipex.child_2 < 0)
        	return (perror("Fork: "));
		if(!pipex.child_2)
			child_proc_2(pipex, argv, envp);
		close(pipex.infile);
		close(pipex.outfile);

		waitpid(pipex.child_1, NULL, 0);
		waitpid(pipex.child_2, NULL, 0);

		// printf("%d %d", pipex.infile, pipex,outfile);
		return (0);
	}
	return (0); // error
}
