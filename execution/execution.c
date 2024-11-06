/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:57:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/06 12:24:04 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_middle_command(t_ast *node, t_ms *ms, int *pipefd, int *write_pipe)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ms->exit_code = 1;
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{
		if ((dup2(pipefd[0], 0) == -1) || (dup2(write_pipe[1], 1) == -1))
		{
			perror("dup2 for pipe input failed");
			close_array_fds(ms);
			ms->exit_code = 1;
			exit(EXIT_FAILURE);
		}
		close_array_fds(ms);
		if (redirection(node) == -1)
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[0]);
	close_and_change_array(ms, &write_pipe[1]);
}

void	execute_last_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ms->exit_code = 1;
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[0], 0) == -1)
		{
			perror("dup2 last cmd fails");
			close_array_fds(ms);
			ms->exit_code = 1;
			exit(EXIT_FAILURE);
		}
		close_array_fds(ms);
		if (redirection(node) == 1)
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[0]);
}

void	execute_first_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ms->exit_code = 1;
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[1], 1) == -1)
		{
			perror("dup2 first cmd fails");
			close_array_fds(ms);
			ms->exit_code = 1;
			exit(EXIT_FAILURE);
		}
		close_array_fds(ms);
		if (redirection(node) == -1)
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[1]);
}

void	execute_command(t_ast *node, t_ms *ms)
{
	if (is_builtin(node))
		exec_builtin(ms, node);
	else
	{
		node->pid = fork();
		if (node->pid == -1)
		{
			perror("fork error");
			ms->exit_code = 1;
			return ;
		}
		else if (node->pid == 0)
		{
			if (redirection(node) == -1)
				exit(EXIT_FAILURE);
			child_process(ms, node);
			exit(EXIT_SUCCESS);
		}
	}
}

void	execute_pipe(t_ast *node, t_ms *ms, int *write_pipe)
{	
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe2 failure");
		ms->exit_code = 1;
		ms->stop = 1;
		return ;
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND && ms->stop == 0)
		execute_middle_command(node->right, ms, pipefd, write_pipe);
	close_multiple_fds(pipefd);
}

void	execute_first_pipe(t_ast *node, t_ms *ms)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe1 failure");
		ms->exit_code = 1;
		ms->stop = 1;
		return ;
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
	{
		fprintf(stderr, "first cmd %s\n", *node->left->exp_value);
		execute_first_command(node->left, ms, pipefd);
	}
	if (node->right && node->right->type == T_CMND && ms->stop == 0)
	{
		fprintf(stderr, "last cmd %s\n", *node->right->exp_value);
		execute_last_command(node->right, ms, pipefd);
	}
	close_multiple_fds(pipefd);
}

// WIFEXITED and WEXITSTATUS are macros
void	ft_waiting(t_ast *node, t_ms *ms)
{
	static int	status;

	if (node->pid != -1 && node->type == T_CMND)
	{
		if (node->type == T_CMND)
			waitpid(node->pid, &status, 0);
		if (WIFEXITED(status))
			ms->exit_code = WEXITSTATUS(status);
	}
	if (node->left)
		ft_waiting(node->left, ms);
	if (node->right)
		ft_waiting(node->right, ms);
}

void	signal_handler_exec(void);

void	execute_ast(t_ast *node, t_ms *ms)
{
	int	i;

	i = 0;
	signal_handler_exec();
	while (i < 1024)
		ms->fds[i++] = -1;
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_first_pipe(node, ms);
	else if (node->type == T_CMND)
		execute_command(node, ms);
	ft_waiting(node, ms);
}
