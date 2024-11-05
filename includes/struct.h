


//tokens
typedef enum e_token_type
{
	T_CMND,			//command names, filenames, and arguments.
	T_IN_REDIR,		//input redirection <
	T_OUT_REDIR,		//output redirection >
	T_HERE_DOC,		//Heredoc <<
	T_APEND,		//Append >>
	T_PIPE,			//pipe
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char			*value;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

//parsing
typedef enum e_io_type
{
	T_IN,
	T_OUT,
	T_HEREDOC,
	T_APPEND,
}	t_io_type;

typedef struct s_io
{
	t_io_type	type;
	char		*value;
	int		heredoc_fd;
	struct s_io	*prev;
	struct s_io	*next;
}	t_io;

typedef struct s_ast
{
	t_token_type		type;
	char		*value;
	char		**exp_value;
	t_io		*io_list;
	int		pid;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//execution
typedef struct s_exec
{
    int     	pipefd[2];
    int		num_cmds;
    int		index;
    int		*pids;
}   t_exec;

//main struct
typedef struct s_ms
{
	char    	**my_envp;	// Environment variables
	int     	exit_code;	// Exit code for the shell
	int     	envp_size;	// Size of the environment variables
	int		stop;	//indicate the minishell should stop and start new round
	int		quit;	//indicate minishell should quit
	char    	*pwd;		// Current working directory
	char		*old_pwd;
	int		fds[1024];
	t_token		*tokens;	//linked list of tokens
	t_ast   	*ast;		// Abstract syntax tree for the command(s)
}	t_ms;

typedef struct s_parsing
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe;
}	t_parsing;
