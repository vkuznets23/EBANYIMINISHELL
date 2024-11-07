/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:55:09 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/07 14:08:06 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Updated handle_cases function to manage quotes
static void	handle_cases(char **clean, char *ins, size_t *i, t_ms *ms)
{
	if (ft_isquote(ins[*i]) || (ins[*i] == '$' && (ins[*i + 1] == '"' || ins[*i + 1] == '\'')))
	{
		if (ins[*i] == '$') // Handle cases like $"..."
			(*i)++;
		handle_quoted_literal(clean, ins, i, ms);
	}
	else if (ins[*i] == '$' && ft_isdelim(ins[*i + 1]))
		handle_normal_char(clean, ins, i, ms);
	else if (ins[*i] == '$' && ins[*i + 1] == '?')
		handle_exit_code(clean, i, ms);
	else if (ins[*i] == '$' && (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_'))
		handle_envir(clean, ins, i, ms);
	else
		handle_normal_char(clean, ins, i, ms);
}

// Expand argument with variable and quote handling
char	*expand_argument(char *arg, t_ms *ms)
{
	char	*expanded_arg;
	size_t	j;

	expanded_arg = ft_strdup("");
	if (!expanded_arg)
		malloc_parent_failure(ms);
	j = 0;
	while (arg[j])
		handle_cases(&expanded_arg, arg, &j, ms);
	return (expanded_arg);
}

//has 31 line
void	expand_ast(t_ast *node, t_ms *ms)
{
	size_t	i;
	char	*new_value;
	t_io	*temp_io;

	if (!node)
		return ;
	i = 0;
	new_value = NULL;
	if (ft_strncmp(node->value, "cd", 3))
	{
		free(node->value);
		node->value = ft_strdup(node->exp_value[1]);
	}

	while (node->exp_value && node->exp_value[i])
	{
		new_value = expand_argument(node->exp_value[i], ms);
		if (!new_value)
			malloc_parent_failure(ms);
		free(node->exp_value[i]);
		node->exp_value[i] = new_value;
		i++;
		temp_io = node->io_list;
		while (temp_io && temp_io->value && temp_io->type != T_HEREDOC)
		{
			new_value = expand_argument(temp_io->value, ms);
			if (!new_value)
				malloc_parent_failure(ms);
			free(temp_io->value);
			temp_io->value = new_value;
			temp_io = temp_io->next;
		}
	}
	if (node->left)
		expand_ast(node->left, ms);
	if (node->right)
		expand_ast(node->right, ms);
}
