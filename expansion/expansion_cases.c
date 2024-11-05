/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:14:57 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 16:06:11 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Handle environment variable expansion //has 27 lines
void	handle_envir(char **clean, char *ins, size_t *i, t_ms *ms)
{
	size_t	start;
	size_t	len;
	char	*envir_name;
	char	*envir_value;
	char	*temp;

	start = *i + 1;
	while (ft_isalnum(ins[start]) || ins[start] == '_')
		start++;
	len = start - (*i + 1);
	envir_name = ft_substr(ins, *i + 1, len);//MALLOC
	if (!envir_name)
	{
		free(*clean);
		malloc_parent_failure(ms);
	}
	envir_value = envp_exists(envir_name, ms);
	free(envir_name);
	if (envir_value)
	{
		temp = ft_strjoin(*clean, envir_value); //MALLOC
		free(*clean);
		if (!temp)
			malloc_parent_failure(ms);
		*clean = temp;
	}
	*i = start; // Move the index after the environment variable
}

// Handle exit code expansion ($?)
void	handle_exit_code(char **clean, size_t *i, t_ms *ms)
{
	char	*substr;
	char	*temp_clean;

	fprintf(stderr, "%d\n", ms->exit_code);
	substr = ft_itoa(ms->exit_code); //MALLOC
	if (!substr)
	{
		free(*clean);
		malloc_parent_failure(ms);
	}
	temp_clean = ft_strjoin(*clean, substr); //MALLOC
	if (!temp_clean)
	{
		free(*clean);
		free(substr);
		malloc_parent_failure(ms);
	}
	free(*clean);
	*clean = temp_clean;
	*i += 2;
	free(substr); //its no longer needed
}

void	handle_normal_char(char **clean, char *ins, size_t *i, t_ms *ms)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str); //MALLOC
	if (!temp)
	{
		free(*clean);
		malloc_parent_failure(ms);
	}
	free(*clean);
	*clean = temp;
	(*i)++;
}

void	handle_quoted_literal(char **clean, char *ins, size_t *i, t_ms *ms)
{
	char	quote_char;

	quote_char = ins[*i];
	(*i)++;
	while (ins[*i] && ins[*i] != quote_char)
	{
		if (quote_char == '"' && ins[*i] == '$')
		{
			if (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_')
				handle_envir(clean, ins, i, ms);
			else if (ins[*i + 1] == '?')
				handle_exit_code(clean, i, ms);
			else
				handle_normal_char(clean, ins, i, ms);
		}
		else
			handle_normal_char(clean, ins, i, ms);
	}
	if (ins[*i] == quote_char)
		(*i)++;
}
