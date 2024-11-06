/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:56:49 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/06 12:24:35 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_fd(int *close_fd)
{
	if (*close_fd != -1)
	{
		close(*close_fd);
		*close_fd = -1;
	}
}

void	close_and_change_array(t_ms *ms, int *fd)
{
	int	i;

	i = 0;
	while (i < 1024)
	{
		if (ms->fds[i] == *fd)
		{
			close_fd(fd);
			ms->fds[i] = -1;
		}
		i++;
	}
}

void	close_multiple_fds(int *pipefd)
{
	if (pipefd)
	{
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
	}
}

void	close_array_fds(t_ms *ms)
{
	int	i;

	i = 0;
	while (i < 1024 && ms->fds[i] != -1)
	{
		close(ms->fds[i]);
		ms->fds[i] = -1;
		i++;
	}
}

void	add_to_array(t_ms *ms, int fd)
{
	int	i;

	i = 0;
	while (i < 1024)
	{
		if (ms->fds[i] == -1)
		{
			ms->fds[i] = fd;
			return ;
		}
		i++;
	}
}
