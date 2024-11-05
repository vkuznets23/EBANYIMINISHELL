/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:48:53 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 13:27:52 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	content_check(const char *arg, t_ms *ms)
{
	int		i;
	char	*content;

	i = 0;
	content = name_exists(arg, ms);
	while (arg[i] != '=' && arg[i] != '\0')
		i++;
	if (arg[i] == '=' && arg[i + 1] == '\0' && *content == '\0')
		return (1);
	else if (arg[i] == '=' && arg[i + 1] != '\0')
		return (1);
	return (0);
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
	return (0);
}

void	envp_update(t_ms *ms, char *content)
{
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (content[size] != '=')
		size++;
	while (i < ms->envp_size)
	{
		if (!ft_strncmp(ms->my_envp[i], content, size))
		{
			free(ms->my_envp[i]);
			ms->my_envp[i] = ft_strdup(content);
			if (!ms->my_envp[i])
				perror("envp update error");
			return ;
		}
		i++;
	}
}

void	envp_add(t_ms *ms, char *content)
{
	char	**new_envp;
	int		i;

	ms->envp_size += 1;
	new_envp = malloc((ms->envp_size + 1) * sizeof(char *));
	if (!new_envp)
		malloc_parent_failure(ms);
	i = 0;
	while (i < ms->envp_size - 1)
	{
		new_envp[i] = ft_strdup(ms->my_envp[i]);
		if (!new_envp[i])
			handle_allocation_failure(new_envp, i, ms);
		i++;
	}
	new_envp[i] = ft_strdup(content);
	if (!new_envp[i])
		handle_allocation_failure(new_envp, i, ms);
	new_envp[i + 1] = NULL;
	free_array(ms->my_envp);
	ms->my_envp = new_envp;
}

void	builtin_export(t_ms *ms, char **cmd, int i)
{
	while (cmd[i] != NULL)
	{
		if (!error_check(cmd[i]))
		{
			if (name_exists(cmd[i], ms) != NULL && content_check(cmd[i], ms))
				envp_update(ms, cmd[i]);
			else if (name_exists(cmd[i], ms) == NULL)
				envp_add(ms, cmd[i]);
		}
		else
		{
			fprintf(stderr, "export: not a valid identifier\n");
			ms->exit_code = 1;
		}
		i++;
	}
}
