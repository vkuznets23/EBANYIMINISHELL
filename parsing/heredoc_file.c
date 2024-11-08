/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:43:50 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/07 11:09:50 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	create_random_name(char *name, char *temp)
{
	int		fd_urandom;
	int		read_bits;
	char		*base;
	unsigned char	bit;

	fd_urandom = open("/dev/urandom", O_RDONLY);
	if (fd_urandom == -1)
		return (1);
	read_bits = read(fd_urandom, temp, 20);
	close(fd_urandom);
	if (read_bits < 0)
		return (1);
	base = "abcdefghijklmnopqrstuvwxyz0123456789";
	while (read_bits--)
	{
		bit = *temp;
		*name = base[bit % ft_strlen(base)];
		temp++;
		name++;
	}
	*name = '\0';
	return (0);
}

int	ft_createfile(int *fd_write, int *fd_read)
{
	char	filename[64];
	char	gen_name[21];
	char	temp[20];

	ft_strlcpy(filename, "/tmp/ms_", 64);
	if (create_random_name(gen_name, temp))
	{
		printf("minishell: Error creating random name\n");
		return (1);
	}
	ft_strlcat(filename, gen_name, 64);
	*fd_write = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	*fd_read = open(filename, O_RDONLY);
	if (*fd_write == -1 || *fd_read == -1)
	{
		printf("minishell: Error opening heredoc file\n");
		if (*fd_write != -1)
			close(*fd_write);
		if (*fd_read != -1)
			close(*fd_read);
		return (1);
	}
	unlink(filename);
	return (0);
}
