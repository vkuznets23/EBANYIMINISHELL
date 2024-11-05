/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:59:01 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 13:28:15 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//32 lines
int	envp_remove(t_ms *ms, char *content)
{
	int		i;
	int		j;
	int		len;
	int		new_size;
	char	**new_envp;

	len = ft_strlen(content);
	new_size = ms->envp_size - 1;
	new_envp = malloc((new_size + 1) * sizeof(char *));
	if (!new_envp)
		malloc_parent_failure(ms);
	i = 0;
	j = 0;
	while (ms->my_envp[j])
	{
		if (!ft_strncmp(ms->my_envp[j], content, len)
			&& (ms->my_envp[j][len] == '=' || ms->my_envp[j][len] == '\0'))
		{
			j++;
			continue ;
		}
		new_envp[i] = ft_strdup(ms->my_envp[j]);
		if (!new_envp[i])
			handle_allocation_failure(new_envp, i, ms);
		i++;
		j++;
	}
	new_envp[i] = NULL;
	free_array(ms->my_envp);
	ms->my_envp = new_envp;
	ms->envp_size = new_size;
	return (1);
}

static int	error_check(char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[i]) || str[i] == '=')
		return (1);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (1);
		i++;
	}
	if (str[i] != '\0')
		return (1);
	return (0);
}

char	*name_exists(const char *arg, t_ms *ms)
{
	int		i;
	int		len;

	len = 0;
	while (arg[len] != '\0' && arg[len] != '=')
		len++;
	i = 0;
	while (ms->my_envp[i])
	{
		if (ft_strncmp(ms->my_envp[i], arg, len) == 0
			&& (ms->my_envp[i][len] == '\0' || ms->my_envp[i][len] == '='))
			return (ms->my_envp[i]);
		i++;
	}
	return (NULL);
}

void	builtin_unset(t_ms *ms, char **cmd)
{
	int	i;

	i = 1;
	while (cmd[i] != NULL)
	{
		if (!error_check(cmd[i]))
		{
			if (name_exists(cmd[i], ms) != NULL)
			{
				if (envp_remove(ms, cmd[i]) == -1)
					return ;
			}
		}
		else
		{
			printf("unset: %s: not a valid identifier\n", cmd[i]);
			ms->exit_code = 1;
		}
		i++;
	}
}
