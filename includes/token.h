#ifndef TOKEN_H
# define TOKEN_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

//token_create.c
t_token	*create_token(t_token_type type, char *value);
void	ft_free_token(t_token *head);
void	add_token_to_list(t_token **lst, t_token *new_token);

#endif
