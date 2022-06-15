/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbeck <wbeck@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 15:48:39 by wbeck             #+#    #+#             */
/*   Updated: 2022/06/16 00:22:58 by wbeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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

void	process_here_doc(t_pipex *pipex, char *argv)
{
	int		here_doc;
	char	*part_line;

	here_doc = open(".heredoc", O_CREAT | O_TRUNC | O_WRONLY, 0000644);
	if (here_doc < 0)
		errors("Open failed on here_doc");
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &part_line) < 0)
			exit(1);
		if (!ft_strncmp(argv, part_line, ft_strlen(argv) + 1))
			break ;
		write(here_doc, part_line, ft_strlen(part_line));
		write(here_doc, "\n", 1);
		free(part_line);
	}
	free(part_line);
	close(here_doc);
	pipex->infile = open(".heredoc", O_RDONLY);
	if (pipex->infile < 0)
	{
		unlink(".heredoc");
		errors("Open failed on input file");
	}
}

int	args_in(char *arg, t_pipex *pipex)
{
	if (arg && !ft_strncmp("here_doc", arg, 9))
	{
		pipex->here_doc = 1;
		return (6);
	}
	else
	{
		pipex->here_doc = 0;
		return (5);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	if (argc < args_in(argv[1], &pipex))
		return (0);
	pipex.here_doc = 0;
	open_files(&pipex, argc, argv);
	pipex.count_cmds = argc - 3 - pipex.here_doc;
	pipex.ends = (int *)malloc(sizeof(int) * (pipex.count_cmds - 1) * 2);
	if (!pipex.ends)
		errors("Out of memory\n");
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
	return (0);
}
