/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:24:56 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/08 14:25:49 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * find_executable - Searches for the full path of a command in the directories
 * listed in PATH.
 * @cmd: The command to search for (e.g., "ls").
 * @path: The PATH environment variable, containing colon-separated directories.
 * @ms: A pointer to the shell structure containing environment variables.
 *
 * This function splits the PATH variable into individual directories, appends
 * the command to each directory, and checks if the resulting path exists and
 * is executable using access(). If a valid path is found, it's returned.
 *
 * Returns:
 * A string representing the full path to the executable if found, or NULL.
 */
char	*find_executable(char *cmd, char *path)
{
	char	**envp_paths;
	char	*exec;
	int		i;

	i = 0;
	envp_paths = ft_split(path, ':');
	if (!envp_paths)
		return (NULL);
	while (envp_paths[i])
	{
		exec = ft_strjoin(envp_paths[i++], cmd);
		if (!exec)
			return (NULL);
		if (access(exec, F_OK) == 0)
		{
			free_array(envp_paths);
			return (exec);
		}
		free(exec);
	}
	free_array(envp_paths);
	return (NULL);
}

/**
 * build_executable - Constructs the full path to an executable.
 * @node: The AST node containing the command to execute.
 * @ms: A pointer to the shell structure containing environment variables.
 *
 * This function checks if the command is an absolute or relative path. If not,
 * it uses the PATH environment variable to search for the command in
 * the directories listed in PATH. The full path is returned if found.
 *
 * Returns:
 *   The full path to the executable if found, or NULL if not found.
 */
char	*build_executable(t_ast *node, t_ms *ms)
{
	char	*path;
	char	*binary;
	char	*find_exec;

	if (!node || !*node->value)
		return (NULL);
	binary = node->exp_value[0];
	if (!ft_strchr(binary, '/'))
	{
		binary = ft_strjoin("/", binary);
		if (!binary)
			return (NULL);
		path = envp_exists("PATH", ms);
		if (!path)
		{
			free(binary);
			return (NULL);
		}
		find_exec = find_executable(binary, path);
		free(binary);
		if (!find_exec)
			return (NULL);
		return (find_exec);
	}
	return (binary);
}
