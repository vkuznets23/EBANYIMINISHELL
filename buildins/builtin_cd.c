/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:10:44 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/06 11:25:02 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*find_the_path(char *str, t_ms *ms)
{
	char	*p;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (ms->my_envp[i])
	{
		if (!ft_strncmp(ms->my_envp[i], str, len))
		{
			p = ft_substr(ms->my_envp[i], len, ft_strlen(ms->my_envp[i]) - len);
			if (!p)
				malloc_parent_failure(ms);
			return (p);
		}
		i++;
	}
	return (NULL);
}

static int	change_dir_path(t_ms *ms, char *str)
{
	char	*tmp;
	int		ret;

	free(ms->old_pwd);
	ms->old_pwd = ms->pwd;
	tmp = find_the_path(str, ms);
	if (!tmp)
	{
		ms->pwd = NULL;
		return (-1);
	}
	ret = chdir(tmp);
	free(tmp);
	if (ret != 0)
	{
		ms->pwd = NULL;
		return (ret);
	}
	ms->pwd = getcwd(NULL, 0);
	if (!ms->pwd)
		malloc_parent_failure(ms);
	return (ret);
}

static void	update_pwds(t_ms *ms)
{
	char	*tmp;

	tmp = ft_strdup(ms->pwd);
	if (!tmp)
		malloc_parent_failure(ms);
	free(ms->old_pwd);
	ms->old_pwd = tmp;
	free(ms->pwd);
	ms->pwd = getcwd(NULL, 0);
	if (!ms->pwd)
		malloc_parent_failure(ms);
}

static int	arg_count(t_ast *ast)
{
	int	i;

	i = 0;
	while (ast->exp_value[i])
		i++;
	return (i);
}

void	builtin_cd(t_ms *ms, t_ast *ast, char *cmd)
{
	int	ret;

	if (arg_count(ast) > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return ;
	}
	if (!cmd || (ft_strncmp(cmd, "~", 2) == 0))
		ret = change_dir_path(ms, "HOME=");
	else if (ft_strncmp(cmd, "-", 2) == 0)
	{
		ret = chdir(ms->old_pwd);
		printf("%s\n", ms->old_pwd);
	}
	else if (ft_strncmp(cmd, "..", 3) == 0)
		ret = chdir("..");
	else
		ret = chdir(cmd);
	if (ret != 0)
	{
		printf("minishell: cd: %s: No such file or directory\n", cmd);
		return ;
	}
	if (cmd && (ft_strncmp(cmd, "~", 2) != 0))
		update_pwds(ms);
}
