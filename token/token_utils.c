/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:15:12 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/06 11:29:20 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_token(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_isoperator(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
		return (2);
	if (*str == '|' || *str == '<' || *str == '>')
		return (1);
	return (0);
}

t_token_type	ft_operator_type(char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0)
		return (T_HERE_DOC);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (T_APEND);
	else if (*str == '<')
		return (T_IN_REDIR);
	else if (*str == '>')
		return (T_OUT_REDIR);
	else
		return (T_PIPE);
}

char	*ft_strndup(const char *src, int i)
{
	char	*dest;
	int		j;

	dest = (char *)malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	j = -1;
	while (++j < i)
		dest[j] = src[j];
	dest[i] = '\0';
	return (dest);
}
