/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:49:46 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/07 13:15:34 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char *token_type_to_str(t_token_type type);

void	parsing_malloc_failure(t_ms *ms, t_parsing *data, t_ast *node)
{
	printf("minishell: cannot allocate memory\n");
	clean_ms(ms);
	ft_free_token(ms->tokens);
	ft_free_ast(data->left);
	ft_free_ast(data->right);
	if (node)
		ft_free_ast_node(node);
	exit(1);
}

t_ast	*parsing_error(t_ms *ms, t_ast *ast, t_ast *node, t_token *token)
{
	const char *str;

	printf("minishell: syntax error near unexpected token `");
	if (token)
	{
		str = token_type_to_str(token->type);
		printf("%s'\n", str);
	}
	else
		printf("newline'\n");
	ms->stop = 1;
	ft_free_token(ms->tokens);
	ft_free_ast(ast);
	ft_free_ast(node);
	return (NULL);
}

void	init_parsing_struct(t_parsing *data)
{
	data->left = NULL;
	data->pipe = NULL;
	data->right = NULL;
}

void	combine_pipeline(t_parsing *data)
{
	data->pipe->left = data->left;
	data->pipe->right = data->right;
	data->left = data->pipe;
	data->right = NULL;
	data->pipe = NULL;
}
