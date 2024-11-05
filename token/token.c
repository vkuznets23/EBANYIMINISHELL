#include "../includes/minishell.h"

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n'
	|| c == '\v' || c == '\f' || c == '\r')
        return (1);
    return (0);
}

int ft_isoperator(char *str)
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

char *ft_strndup(const char *src, int i)
{
    char	*dest;
    int		j;

    dest = (char *)malloc(sizeof(char) * (i + 1));
    if (!dest)
        return NULL;
    j = -1;
    while (++j < i)
        dest[j] = src[j];
    dest[i] = '\0';
    return dest;
}

void	create_operator_token(char **str, t_token **lst, t_token_type type, t_ms *ms)
{
	t_token *node;

	node = create_token(type, NULL);
	if (!node)
	{
		ms->quit = 1;
		return ; //error handling needed
	}
	add_token_to_list(lst, node);
	if (node->type == T_HERE_DOC || node->type == T_APEND)
		(*str)++;
	(*str)++;
}

//how to handle empty quotes? create an emptry str?
void	handle_quote(char **str, int *i, t_ms *ms)
{
	char	type;

	type = (*str)[*i];
	(*i)++;
	while ((*str)[*i])
	{
		if ((*str)[*i] == type)
			break;
		else
			(*i)++;
	}
	if (!(*str)[*i])  //fix error handling for incorrect quotes
	{
		printf("minishell: No closing quote\n");
		ms->stop = 1;
		return ;
	}
	(*i)++;
	while ((*str)[*i] && !ft_isspace((*str)[*i]) && !ft_isoperator((*str)))
	{
		if ((*str)[*i] == '\'' || (*str)[*i] == '"')
		{
			handle_quote(str, i, ms);
			break;
		}
		else
			(*i)++;
	}
}

void	create_argument_token(char **str, t_token **lst, t_token_type type, t_ms *ms)
{
	int 	i;
	char	*line;
	t_token	*node;

	i = 0;
	while ((*str)[i] && !ft_isoperator(((*str)+i)) && !ft_isspace((*str)[i]))
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			handle_quote(str, &i, ms); //WIP
		else
			i++;
		if (ms->stop == 1)
			return;
	}
	line = ft_strndup(*str, i);
	if (!line)
	{
		ms->quit = 1;
		return ; //error handling needed
	}
	node = create_token(type, line);
	if (!node)
	{
		free(line);
		ms->quit = 1;
		return ; //error handling needed
	}
	add_token_to_list(lst, node);
	(*str) += i;
}

t_token	*ft_tokenize(char *str, t_ms *ms)
{
	t_token *lst;
	char	*temp;

	lst = NULL;
	temp = str;
	while (*str && !ms->stop && !ms->quit)
	{
		while (ft_isspace(*str))
			str++;
		if (ft_isoperator(str))
			create_operator_token(&str, &lst, ft_operator_type(str), ms);
		else
			create_argument_token(&str, &lst, T_CMND, ms);
		while (ft_isspace(*str))
			str++;
	}
	if (ms->stop || ms->quit)
	{
		ft_free_token(lst);
		if (ms->quit)
		{
			clean_ms(ms);
			free(temp);
			printf("minishell: cannot allocate memory\n");
			exit(1);
		}
	}
	free(temp);
	return (lst);
}
