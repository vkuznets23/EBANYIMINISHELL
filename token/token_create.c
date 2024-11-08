/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:15:04 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/06 11:26:00 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token_to_list(t_token **lst, t_token *new_token)
{
	t_token	*temp;

	if (!*lst)
		*lst = new_token;
	else
	{
		temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

void	crt_operator_tkn(char **str, t_token **lst, t_token_type type, t_ms *ms)
{
	t_token	*node;

	node = create_token(type, NULL);
	if (!node)
	{
		ms->quit = 1;
		return ;
	}
	add_token_to_list(lst, node);
	if (node->type == T_HERE_DOC || node->type == T_APEND)
		(*str)++;
	(*str)++;
}
