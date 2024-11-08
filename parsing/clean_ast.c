/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:31:09 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/06 16:08:36 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_io_list(t_io *io_list)
{
	t_io	*temp;

	while (io_list)
	{
		free(io_list->value);
		if (io_list->type == T_HEREDOC && io_list->heredoc_fd != -1)
			close(io_list->heredoc_fd);
		temp = io_list;
		io_list = io_list->next;
		free(temp);
	}
}

void	ft_free_ast_node(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	free(node->value);
	i = 0;
	while (node->exp_value && node->exp_value[i])
	{
		free(node->exp_value[i]);
		i++;
	}
	free(node->exp_value);
	ft_free_io_list(node->io_list);
	free(node);
}

void	ft_free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->left)
		ft_free_ast(root->left);
	if (root->right)
		ft_free_ast(root->right);
	ft_free_ast_node(root);
}
