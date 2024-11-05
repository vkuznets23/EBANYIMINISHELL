/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:12:19 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 12:12:48 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_flags(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (1);
	else
		i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	builtin_echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 1;
	flag = false;
	while (cmd[i] && check_flags(cmd[i]) == 0)
	{
		flag = true;
		i++;
	}
	if (!cmd[i] && flag == false)
		ft_putchar_fd('\n', 1);
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], 1);
		if (cmd[i + 1] == NULL && flag == false)
			ft_putchar_fd('\n', 1);
		else if (cmd[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
}
