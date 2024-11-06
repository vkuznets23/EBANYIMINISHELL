#ifndef EXECUTION_H
# define EXECUTION_H

// Libraries
/*#include <stdbool.h> //boolians
#include <unistd.h> //malloc
#include <fcntl.h> //fds
#include <sys/wait.h> //waitpid*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <errno.h>

//EXPANSION
char	*expand_argument(char *arg, t_ms *ms);
void	expand_ast(t_ast *node, t_ms *ms);
void	handle_envir(char **clean, char *ins, size_t *i, t_ms *ms);
void	handle_exit_code(char **clean, size_t *i, t_ms *ms);
void	handle_normal_char(char **clean, char *ins, size_t *i, t_ms *ms);
void	handle_quoted_literal(char **clean, char *ins, size_t *i, t_ms *ms);
int	ft_isquote(char c);
int	ft_isdelim(char c);

//BUILTINS
void	builtin_cd(t_ms *ms, t_ast *ast, char *cmd);
void	builtin_echo(char **cmd);
void	builtin_env(t_ms *ms);
void	builtin_exit(t_ms *ms, char **cmd);
void	builtin_export(t_ms *ms, char **cmd, int i);
void	builtin_unset(t_ms *ms, char **cmd);

//ENVP
void	init_envp(t_ms *ms, char **envp);
char *envp_exists(char *name, t_ms *ms);
char	*name_exists(const char *arg, t_ms *ms);

//EXECUTION
bool	is_builtin(t_ast *ast);
void	exec_builtin(t_ms *ms, t_ast *ast);
void	child_process(t_ms *ms, t_ast *ast);
void	execute_ast(t_ast *node,  t_ms *ms);
void	close_fd(int *close_fd);
void	close_and_change_array(t_ms *ms, int *fd);
void	close_multiple_fds(int *pipefd);
void	close_array_fds(t_ms *ms);
void	add_to_array(t_ms *ms, int fd);
char	*build_executable(t_ast *node, t_ms *ms);

//REDIRECTION
int	redirection(t_ast *node);

//CLEAN_UTILS
void	malloc_parent_failure(t_ms *ms);
void	handle_allocation_failure(char **new_envp, int count, t_ms *ms);

#endif
