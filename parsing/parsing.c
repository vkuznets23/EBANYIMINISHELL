#include "../includes/minishell.h"

void	parsing_malloc_failure(t_ms *ms, t_parsing *data, t_ast *node);

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
		return;
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

int	ft_isredirection(enum e_token_type type)
{
	return (type == T_IN_REDIR || type == T_OUT_REDIR
		|| type == T_HERE_DOC || type == T_APEND);
	exit(1);
}

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
		new_node->value = NULL;;
	new_node->exp_value = NULL;
	new_node->io_list = NULL;
	new_node->pid = -1;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_io_type token_to_io_type(t_token_type type)
{
	if (type == T_IN_REDIR)
		return (T_IN);
	else if (type == T_OUT_REDIR)
		return (T_OUT);
	else if (type == T_HERE_DOC)
		return (T_HEREDOC);
	else
		return (T_APPEND);
}

char	**append_args(char **args, char *to_add, t_ms *ms, t_token **tokens)
{
	int len;
	int i;
	char **new_args;

	len = 0;
	i = -1;
	while (args && args[len])
		len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
	{
		free_array(args);
		ms->stop = 1;
		return (NULL); //WIP ERROR HANDLE
	}
	while (++i < len)
		new_args[i] = args[i];
	new_args[i] = ft_strdup(to_add);
	if (!new_args[i])
	{
		free_array(args);
		free(new_args);
		ms->stop = 1;
		return (NULL);
	}
	new_args[++i] = NULL;
	free(args);
	*tokens = (*tokens)->next;
	return (new_args);
}


// Function to create a new I/O node
t_io	*create_io_node(t_io_type type, char *value)
{
	t_io	*new_io;

	new_io = (t_io *)malloc(sizeof(t_io));
	if (!new_io)
		return (NULL);
	new_io->type = type;
	new_io->value = ft_strdup(value); // Duplicate the value
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

// Function to append an I/O node to the AST node's I/O list
int	add_io_to_ast(t_ast *ast_node, t_io_type io_type, char *io_value, t_token **tokens)
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

//tried to make 1 function that works on all ordres of cmnds and redirections. need more testing
t_ast	*parse_command(t_token **tokens, t_ms *ms, t_parsing *data)
{
	t_ast *node;

	if ((*tokens)->type == T_CMND)
		node = create_ast_node(T_CMND, (*tokens)->value);
	else
		node = create_ast_node(T_CMND, NULL);
	if (!node)
		parsing_malloc_failure(ms, data, NULL);
	if (node->value)
	{
		node->exp_value = append_args(node->exp_value, (*tokens)->value, ms, tokens);
		if (!node->exp_value)
			parsing_malloc_failure(ms, data, node);
	}
	while (*tokens && ((*tokens)->type == T_CMND || ft_isredirection((*tokens)->type)))
	{
		if ((*tokens)->type == T_CMND)
		{
			if (!node->value)
			{
				node->value = ft_strdup((*tokens)->value);
				if(!node->value)
					parsing_malloc_failure(ms, data, node);
			}
			node->exp_value = append_args(node->exp_value, (*tokens)->value, ms, tokens); //prog
			if (!node->exp_value)
				parsing_malloc_failure(ms, data, node);
		}
		else if(ft_isredirection((*tokens)->type) && (*tokens)->next && (*tokens)->next->type ==T_CMND)
		{
			if (add_io_to_ast(node, token_to_io_type((*tokens)->type), (*tokens)->next->value, tokens))
				parsing_malloc_failure(ms, data, node);
		}
		else
		{
			ft_free_ast_node(node);
			return (NULL);
		}
	}
	return (node);
}

void	parsing_malloc_failure(t_ms *ms, t_parsing *data, t_ast *node)
{
	printf("minishell: cannot allocate memory\n");
	//free ms stuff
	clean_ms(ms);
	ft_free_token(ms->tokens);
	//free parsing stuff
	ft_free_ast(data->left);
	ft_free_ast(data->right);
	//free node if ther is one
	if (node)
		ft_free_ast_node(node);
	exit(1);
}

t_ast	*parsing_error(t_ms *ms, t_ast *ast, t_ast *node)
{
	printf("minishell: parsing error\n");
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

t_ast	*parsing_ast(t_token *tokens, t_ms *ms)
{
	t_parsing	data;

	init_parsing_struct(&data);
	if (!tokens || tokens->type == T_PIPE)
		return (parsing_error(ms, NULL, NULL));
	data.left = parse_command(&tokens, ms, &data);
	if (!data.left)
		return (parsing_error(ms, NULL, NULL));
	while (tokens && tokens->type == T_PIPE)
	{
		tokens = tokens->next;
		if (!tokens || tokens->type == T_PIPE)
			return (parsing_error(ms, data.left, NULL));
		data.right = parse_command(&tokens, ms, &data);
		if (!data.right)
			return (parsing_error(ms, data.left, NULL));
		data.pipe = create_ast_node(T_PIPE, NULL);
		if (!data.pipe)
			parsing_malloc_failure(ms, &data, NULL);
		combine_pipeline(&data);
	}
	ft_free_token(ms->tokens);
	ms->tokens = NULL;
	return (data.left);
}
