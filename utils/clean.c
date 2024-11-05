/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:11:45 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 12:06:08 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	malloc_parent_failure(t_ms *ms)
{
	printf("minishell: cannot allocate memory\n");
	ft_free_ast(ms->ast);
	clean_ms(ms);
	exit(1);
}

void	handle_allocation_failure(char **new_envp, int count, t_ms *ms)
{
	while (count > 0)
		free(new_envp[--count]);
	free(new_envp);
	malloc_parent_failure(ms);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	clean_ms(t_ms *ms)
{
	free_array(ms->my_envp);
	ms->my_envp = NULL;
	free(ms->pwd);
	ms->pwd = NULL;
	free(ms->old_pwd);
	ms->old_pwd = NULL;
}
