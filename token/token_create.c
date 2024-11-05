#include "../includes/minishell.h"

void	ft_free_token(t_token *head)
{
	t_token *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token *token; 

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
	t_token *temp;

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

