/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:25:41 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/07 13:42:21 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_options(int error)
{
	if (error == 1)
		ft_putstr_fd(": Permission denied", 2);
	else if (error == 2)
		ft_putstr_fd(": No such file or directory", 2);
	else if (error == 3)
		ft_putstr_fd("Command not found: ", 2);
}

void	error_handler(char *file_name, int error)
{
	ft_putstr_fd("minishell: ", 2);
	if (file_name)
		ft_putstr_fd(file_name, 2);
	error_options(error);
	ft_putstr_fd("\n", 2);
}

// <
static int	ft_in(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_RDONLY);
	if (fd == -1)
	{
		if (access(io_list->value, F_OK))
		{
			error_handler(io_list->value, 2);
			return (-1);
		}
		else if (access(io_list->value, R_OK))
		{
			error_handler(io_list->value, 1);
			return (-1);
		}
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// >
static int	ft_out(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (access(io_list->value, F_OK))
		{
			error_handler(io_list->value, 2);
			return (-1);
		}
		else if (access(io_list->value, W_OK))
		{
			error_handler(io_list->value, 1);
			return (-1);
		}
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

//Appending >>
static int	ft_append(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (access(io_list->value, F_OK)) //noaccess DO I NEED TO ADD CREATING ERMISSION??
		{
			error_handler(io_list->value, 2);
			return (-1);
		}
		else if (access(io_list->value, W_OK))
		{
			error_handler(io_list->value, 1);
			return (-1);
		}
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

//Heredoc <<
static int	ft_heredoc(t_io *io_list)
{
	if (io_list->heredoc_fd == -1)
		return (0);
	if (dup2(io_list->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(io_list->heredoc_fd);
		return (-1);
	}
	close(io_list->heredoc_fd);
	return (0);
}

int	redirection(t_ast *node)
{
	t_io	*current_io;
	int		status;

	current_io = node->io_list;
	status = 0;
	while (current_io)
	{
		if (current_io->type == T_IN)
			status = ft_in(current_io);
		else if (current_io->type == T_OUT)
			status = ft_out(current_io);
		else if (current_io->type == T_APPEND)
			status = ft_append(current_io);
		else if (current_io->type == T_HEREDOC)
			status = ft_heredoc(current_io);
		if (status == -1)
			return (-1);
		current_io = current_io->next;
	}
	return (0);
}
