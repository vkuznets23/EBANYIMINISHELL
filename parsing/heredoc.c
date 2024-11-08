/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:07:32 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/07 11:21:50 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*Step by step
 * 1. Generate a filename by using random bits from /dev/urandom
 * 2.create a temptorary file with the random name to /tmp/ms_$NAME
 * 3. open writing and reading fd for the created file and use
 * unlink to make sure it it destroyed after closing all existing fds of it
 *
 * 4.check if the heredoc is actually used? /if there is anotherredirectin after
 * 	4.A if so normal read
 * 	4.B if not file is not needed and heredoc info is not actully saved anywhere
 * 5.read with readline check for termianting word and EOF and ctrl+c
 * 6.check if the line needs to be expanded or nor based on quotes in the termianting word
 * 7.
 * /
 *
 * bash: syntax error near unexpected token `>>' for missing redirections
 */

void	ft_empty_heredoc(t_ms *ms, t_io *io, char *delim)
{
	char	*line;

	signal_handler_heredoc();
	remove_delim_quotes(delim);
	while (1)
	{
		line = readline(">");
		if (!line || ms_signal || !ft_strcmp(line, delim))
			break ;
		free(line);
		line = NULL;
	}
	if (!line)
		printf("minishell: warning: here-document delimited by end of-file (wanted %s)\n", delim);
	if (ms_signal)
		ms->stop = 1;
	free(line);
	io->heredoc_fd = -1;
	signal_handler_parent();
}

int	ft_heredoc(t_ms *ms, t_io *io)
{
	int	fd_read;
	int	fd_write;

	if (ft_createfile(&fd_write, &fd_read))
	{
		ms->stop = 1;
		return (1);
	}
	if (ft_heredoc_getline(io, io->value, fd_write, ms))
		close(fd_write);
	else
		io->heredoc_fd = fd_read;
	close(fd_write);
	return (0);
}

int	ft_heredoc_used(t_io *io)
{
	io = io->next;
	while (io)
	{
		if (io->type == T_HEREDOC || io->type == T_IN)
			return (1);
		io = io->next;
	}
	return (0);
}

int	ast_heredoc(t_ast *tree, t_ms *ms)
{
	t_io	*io_temp;

	if (ms->stop || !tree)
		return (1);
	io_temp = tree->io_list;
	while (!ms->stop && tree->type == T_CMND && io_temp)
	{
		if (io_temp->type == T_HEREDOC)
		{
			if (ft_heredoc_used(io_temp))
				ft_empty_heredoc(ms, io_temp, io_temp->value);
			else
				ft_heredoc(ms, io_temp);
		}
		io_temp = io_temp->next;
	}
	if (tree->left && !ms->stop)
		ast_heredoc(tree->left, ms);
	if (tree->right && !ms->stop)
		ast_heredoc(tree->right, ms);
	return (0);
}
