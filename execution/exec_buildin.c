/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:15:41 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/07 14:33:33 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_builtin(t_ast *ast)
{
	char	*cmd;
	int		len;

	cmd = ast->exp_value[0]; //executable command
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1)))
		|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
		|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1)))
		return (true);
	return (false);
}

bool	is_child_builtin(t_ast *ast)
{
	char	*cmd;
	int		len;

	cmd = ast->exp_value[0]; //executable command
	len = ft_strlen(cmd);
	if (((len == 3) && !(ft_strncmp("pwd", cmd, len + 1)))
		|| ((len == 3) && !ft_strncmp("env", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("echo", cmd, len + 1)))
		return (true);
	return (false);
}

int	exec_builtin(t_ms *ms, t_ast *ast)
{
	int	ret;

	ret = 0;
	if (!ft_strncmp("echo", ast->exp_value[0], 5))
		builtin_echo(ast->exp_value);
	else if (!ft_strncmp("cd", ast->exp_value[0], 3))
		ret = builtin_cd(ms, ast, ast->exp_value[1]);
	else if (!ft_strncmp("env", ast->exp_value[0], 3))
		builtin_env(ms);
	else if (!ft_strncmp("pwd", ast->exp_value[0], 4))
		printf("%s\n", ms->pwd);
	else if (!ft_strncmp("exit", ast->exp_value[0], 5))
        	builtin_exit(ms, ast->exp_value);
	else if (!ft_strncmp("export", ast->exp_value[0], 7))
		builtin_export(ms, ast->exp_value, 1);
	else if (!ft_strncmp("unset", ast->exp_value[0], 6))
		builtin_unset(ms, ast->exp_value);
	else
		printf("Command not found: %s\n", ast->exp_value[0]);
	return (ret);
}


int	exec_bin(t_ms *ms, t_ast *node)
{
	int		ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);
	fprintf(stderr, "%s\n", cmd_path);
	if (cmd_path)
	{
		if (access(cmd_path, F_OK))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(node->exp_value[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(cmd_path);
			ms->exit_code = 127;
			return (-1);
		}
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
		{
			if (access(cmd_path, X_OK))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(node->exp_value[0], 2);
				ft_putstr_fd(": Permission denied\n", 2);
				ms->exit_code = 126;
				free(cmd_path);
				return (-1);
			}
			else if (access(cmd_path, F_OK))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(node->exp_value[0], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				free(cmd_path);
				return (-1);
			}
			ms->exit_code = 1;
			free(cmd_path);
			return (-1); //that means fail
		}
	}
	else
	{
		ft_putstr_fd(node->exp_value[0], 2);
		ft_putstr_fd(": command not found\n", 2);

		ft_free_ast(ms->ast);
		clean_ms(ms);
		exit(127);
	}
	free(cmd_path);
	return (ret);
}

// Function to handle command execution in the child process
void	child_process(t_ms *ms, t_ast *ast)
{
	if (is_child_builtin(ast) || is_builtin(ast))
	{
		if (exec_builtin(ms, ast) == -1) //bacause none of child functions are here
			exit(EXIT_FAILURE);
		ft_free_ast(ms->ast);
		clean_ms(ms);
	}
	else
	{
		if (exec_bin(ms, ast) == -1)
			exit(ms->exit_code);
	}
}

