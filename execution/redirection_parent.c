#include "../includes/minishell.h"

void	error_options(int error);
void	error_handler(char *file_name, int error);

// Input redirection >
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
	close(fd);
	return (0);  // Success
}

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
	close(fd);
	return (0);
}

static int	ft_append(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
	close(fd);
	return (0);
}


int	redirection_parent(t_ast *node)
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
		if (status == -1)
			return (-1);
		current_io = current_io->next;
	}
	return (0);
}
