/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:01:48 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/06 16:14:04 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*create_ast_node(t_token_type type, char *str)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	if (str)
	{
		new_node->value = ft_strdup(str);
		if (!new_node->value)
		{
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->exp_value = NULL;
	new_node->io_list = NULL;
	new_node->pid = -1;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_io	*create_io_node(t_io_type type, char *value)
{
	t_io	*new_io;

	new_io = (t_io *)malloc(sizeof(t_io));
	if (!new_io)
		return (NULL);
	new_io->type = type;
	new_io->value = ft_strdup(value);
	if (!new_io->value)
	{
		free(new_io);
		return (NULL);
	}
	new_io->heredoc_fd = -1;
	new_io->next = NULL;
	new_io->prev = NULL;
	return (new_io);
}

int	add_io(t_ast *ast_node, t_io_type io_type, char *io_value, t_token **tokens)
{
	t_io	*new_io;
	t_io	*last;

	if (!ast_node)
		return (1);
	new_io = create_io_node(io_type, io_value);
	if (!new_io)
		return (1);
	if (!ast_node->io_list)
		ast_node->io_list = new_io;
	else
	{
		last = ast_node->io_list;
		while (last->next)
			last = last->next;
		last->next = new_io;
		new_io->prev = last;
	}
	*tokens = (*tokens)->next;
	*tokens = (*tokens)->next;
	return (0);
}
