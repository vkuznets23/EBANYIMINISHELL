/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:25:28 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/05 15:15:53 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

int	ms_signal = 0;

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);
t_token		*ft_tokenize(char *str, t_ms *ms);
void		print_tokens(t_token *tokens);
void		signal_handler_parent(void);

void	init_minishell(t_ms *ms, char **envp)
{
	struct termios	term;

	ms->envp_size = 0;
	init_envp(ms, envp);
	ms->exit_code = 0;
	ms->envp_size = 0;
	ms->stop = 0;
	ms->quit = 0;
	ms->pwd = getcwd(NULL, 0);
	ms->old_pwd = NULL;
	ms->ast = NULL;
	ms->tokens = NULL;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	reset_ms(t_ms *ms)
{
	ms->stop = 0;
	ms->quit = 0;
	ms->ast = NULL;
	ms->tokens = NULL;
	ms_signal = 0;
}

int	main(int ac, char **av, char **envp)
{
	char	prompt[1000] = "minishell> ";
	char	*str;
	t_ms	ms;

	(void)ac;
	(void)av;
	init_minishell(&ms, envp);
	while (1)
	{
		signal_handler_parent();
		reset_ms(&ms);
		str = readline(prompt);
		if (ms_signal)
		{
			ms_signal = 0;
			ms.exit_code = 130;
		}
		if (!str)
		{
			printf("exit\n");
			break;
		}
		if (!*str)
		{
			free(str);
			continue ;
		}
		else
			add_history(str);
		ms.tokens = ft_tokenize(str, &ms);
		if (ms.stop)
			continue ;
		print_tokens(ms.tokens); //DEBUG
		//creating the ast tree;
		ms.ast = parsing_ast(ms.tokens, &ms);
		if (ms.stop)
			continue ;
		expand_ast(ms.ast, &ms);
		print_ast_tree(ms.ast);  //debug
		ast_heredoc(ms.ast, &ms);
		if (ms.stop)
		{
			ft_free_ast(ms.ast);
			continue ;
		}
		execute_ast(ms.ast, &ms);
		ft_free_ast(ms.ast);
	}
	clean_ms(&ms);
}
