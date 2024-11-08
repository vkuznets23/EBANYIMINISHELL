#ifndef MINISHELL_H
# define MINISHELL_H

extern int ms_signal;

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../Libft/libft.h"
#include "struct.h"
#include "token.h"
#include "parsing.h"
#include "execution.h"

void	free_array(char **array);
void	clean_ms(t_ms *ms);

//signals
void	signal_handler_child();
void	signal_handler_exec();
void	signal_handler_parent();
void	signal_handler_heredoc();

#endif
