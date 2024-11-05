/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:14:06 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 12:14:10 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

void	builtin_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->my_envp[i] != NULL)
	{
		if (strchr(ms->my_envp[i], '='))
			ft_putendl_fd(ms->my_envp[i], 1);
		i++;
	}
}
