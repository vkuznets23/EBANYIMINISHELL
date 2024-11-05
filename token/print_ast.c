#include <stdio.h>
#include "../includes/minishell.h"

// Function to print indentation for better visualization
void print_indent(int level)
{
    for (int i = 0; i < level; i++)
        printf("  ");
}

// Function to convert t_token_type to a readable string for printing
const char *token_type_to_str(t_token_type type)
{
    switch (type)
    {
        case T_PIPE: return "PIPE (|)";
        case T_IN_REDIR: return "IN REDIR";
        case T_OUT_REDIR: return "OUT_REDIR";
        case T_CMND: return "COMMAND";
	case T_HERE_DOC: return "HEREDOC";
	case T_APEND: return "APPEND";
        default: return "UNKNOWN";
    }
}

// Function to convert t_io_type to a readable string for printing
const char *io_type_to_str(t_io_type type)
{
    switch (type)
    {
        case T_IN: return "Input (<)";
        case T_OUT: return "Output (>)";
        case T_HEREDOC: return "Heredoc (<<)";
        case T_APPEND: return "Append (>>)";
        default: return "UNKNOWN IO";
    }
}

// Recursively print the AST
void print_ast(t_ast *node, int level)
{
    if (!node)
        return;

    // Print current node
    print_indent(level);
    printf("Node Type: %s\n", token_type_to_str(node->type));

    // Print node value if available
    if (node->value)
    {
        print_indent(level);
        printf("  Value: %s\n", node->value);
    }

    // Print expanded arguments if available
    if (node->exp_value)
    {
        print_indent(level);
        printf("  Expanded Args: ");
        for (int i = 0; node->exp_value[i]; i++)
            printf("%s ", node->exp_value[i]);
        printf("\n");
    }

    // Print IO redirections if available
    t_io *io = node->io_list;
    while (io)
    {
        print_indent(level);
        printf("  IO Redirect: %s -> %s\n", io_type_to_str(io->type), io->value);
        io = io->next;
    }

    // Recursively print the left and right child nodes
    if (node->left)
    {
        print_indent(level);
        printf("  Left Child:\n");
        print_ast(node->left, level + 1);
    }
    
    if (node->right)
    {
        print_indent(level);
        printf("  Right Child:\n");
        print_ast(node->right, level + 1);
    }
}

// Wrapper function to print the entire AST
void print_ast_tree(t_ast *root)
{
    printf("AST Tree:\n");
    print_ast(root, 0);
}
// Function to print out tokens and their types
void	print_tokens(t_token *tokens)
{
	while(tokens)  //testing and printing out the tokens
	{
	printf("Token type %s\n", token_type_to_str(tokens->type));
	tokens = tokens->next;
	}
}
