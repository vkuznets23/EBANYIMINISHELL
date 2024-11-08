#ifndef TOKEN_H
# define TOKEN_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

//token_create.c
t_token	*create_token(t_token_type type, char *value);
void	add_token_to_list(t_token **lst, t_token *new_token);
void	crt_operator_tkn(char **str, t_token **lst, t_token_type type, t_ms *ms);

//token_utils.c
void	ft_free_token(t_token *head);
int	ft_isspace(char c);
int	ft_isoperator(char *str);
char	*ft_strndup(const char *src, int i);
t_token_type	ft_operator_type(char *str);

#endif
