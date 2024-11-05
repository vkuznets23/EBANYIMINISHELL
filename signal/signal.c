#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/minishell.h"

//need signal handler for execution. chld funcs should not ignore ctrl+\?


//TO DO! signal handler after heredoc for execution and child process
//curr getting multiple minishell > prompts

void	ft_sigint_parent(int num)
{
	ms_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay(); // Redraw the prompt
}

void	ft_sigint_heredoc(int num)
{
	//ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
	ms_signal = num;
}

int	ft_readline_event(void)
{
	return(0);
}

void	signal_handler_heredoc()
{
	rl_event_hook = ft_readline_event;
	signal(SIGINT, ft_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler_parent()
{
	rl_done = 0;
	rl_event_hook = NULL;
	signal(SIGINT, ft_sigint_parent);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_sigint_exec(int num)
{
	ms_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	ft_sigquit(int num)
{
	ms_signal = num;
	printf("Quit (core dumped)\n");
}

void	signal_handler_exec()
{
	signal(SIGINT, ft_sigint_exec);
	signal(SIGQUIT, ft_sigquit);
}

void	signal_handler_child()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
