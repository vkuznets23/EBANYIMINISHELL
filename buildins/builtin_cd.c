/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:10:44 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/08 14:00:48 by vkuznets         ###   ########.fr       */
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
			{
				fprintf(stderr, "hello\n");
				malloc_parent_failure(ms);
				return NULL;
			}
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
		ft_putstr_fd("minishell: cd: HOME not set\n", 2); //?
		ms->pwd = NULL;
		return (-2);
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

static void	update_pwds(t_ms *ms , char *cmd)
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
	{
		if (errno == ENOMEM)
			malloc_parent_failure(ms);
		else if (errno == ENOENT)
		{
			ft_putstr_fd("cd: error retrieving current directory:", 2);
			ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
			ft_putstr_fd("No such file or directory\n", 2);
			if (ft_strncmp(cmd, "..", 3) == 0)
				ms->pwd = ft_strjoin(ms->old_pwd, "/.."); //MALLOC
			else if (ft_strncmp(cmd, ".", 2) == 0)
				ms->pwd = ft_strjoin(ms->old_pwd, "/."); //MALLOC
			return ;
		}
	}
}

static int	arg_count(t_ast *ast)
{
	int	i;

	i = 0;
	while (ast->exp_value[i])
		i++;
	return (i);
}

int	builtin_cd(t_ms *ms, t_ast *ast, char *cmd)
{
	int	ret;

	if (arg_count(ast) > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (-1);
	}

	fprintf(stderr, "cmd = %s\n", cmd);
	fprintf(stderr, "ast = %s\n", ast->value);
	if (!cmd || (ft_strncmp(cmd, "~", 2) == 0) || !*cmd)
	{
		fprintf(stderr, "1\n");
		ret = change_dir_path(ms, "HOME=");
	}
	else if (ft_strncmp(cmd, "-", 2) == 0)
	{
		ret = chdir(ms->old_pwd);
		printf("%s\n", ms->old_pwd);
	}
	else if (ft_strncmp(cmd, "..", 3) == 0)
		ret = chdir(cmd);
	//check with . -> it should stay at the same dir 
	else
		ret = chdir(cmd);
	if (ret == -1)
	{
		printf("minishell: cd: %s: No such file or directory\n", cmd);
		return (-1);
	}
	if (cmd && (ft_strncmp(cmd, "~", 2) != 0) && *cmd)
		update_pwds(ms, cmd);
	return (ret);
}
