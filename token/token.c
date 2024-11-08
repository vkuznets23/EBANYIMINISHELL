/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:14:40 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/06 11:26:19 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);

static void	error_quit(t_ms *ms, char *str)
{
	clean_ms(ms);
	free(str);
	printf("minishell: cannot allocate memory\n");
	exit(1);
}

static int	error_set(t_ms *ms, char *str, int type)
{
	free(str);
	if (type == 1)
	{
		printf("minishell: No closing quote\n");
		ms->stop = 1;
	}
	else if (type == 2)
		ms->quit = 1;
	return (1);
}

static int	handle_quote(char **str, int *i, t_ms *ms)
{
	char	type;

	type = (*str)[*i];
	(*i)++;
	while ((*str)[*i])
	{
		if ((*str)[*i] == type)
			break ;
		else
			(*i)++;
	}
	if (!(*str)[*i])
		return (error_set(ms, NULL, 1));
	(*i)++;
	while ((*str)[*i] && !ft_isspace((*str)[*i]) && !ft_isoperator((*str)))
	{
		if ((*str)[*i] == '\'' || (*str)[*i] == '"')
		{
			handle_quote(str, i, ms);
			break ;
		}
		else
			(*i)++;
	}
	return (0);
}

static	int	create_arg(char **str, t_token **lst, t_token_type type, t_ms *ms)
{
	char	*line;
	t_token	*node;
	int		i;

	i = 0;
	while ((*str)[i] && !ft_isoperator(((*str) + i)) && !ft_isspace((*str)[i]))
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			handle_quote(str, &i, ms);
		else
			i++;
		if (ms->stop == 1)
			return (1);
	}
	line = ft_strndup(*str, i);
	if (!line)
		return (error_set(ms, NULL, 2));
	node = create_token(type, line);
	if (!node)
		return (error_set(ms, line, 2));
	add_token_to_list(lst, node);
	(*str) += i;
	return (0);
}

t_token	*ft_tokenize(char *str, t_ms *ms)
{
	t_token	*lst;
	char	*temp;

	lst = NULL;
	temp = str;
	while (*str && !ms->stop && !ms->quit)
	{
		while (ft_isspace(*str))
			str++;
		if (ft_isoperator(str))
			crt_operator_tkn(&str, &lst, ft_operator_type(str), ms);
		else
			create_arg(&str, &lst, T_CMND, ms);
		while (ft_isspace(*str))
			str++;
	}
	if (ms->stop || ms->quit)
	{
		ft_free_token(lst);
		if (ms->quit)
			error_quit(ms, temp);
	}
	free(temp);
	return (lst);
}
