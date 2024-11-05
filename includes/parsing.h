#ifndef PARSING_H
# define PARSING_H

#include <stdio.h> //move to main h file
#include <stdlib.h> //move to main h file

int	ast_heredoc(t_ast *tree, t_ms *ms);
void	ft_free_ast(t_ast *root);
t_ast	*parsing_ast(t_token *tokens, t_ms *ms);

#endif
