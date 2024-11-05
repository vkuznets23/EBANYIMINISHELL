#include "../includes/minishell.h"

/*Step by step
 * 1. Generate a filename by using random bits from /dev/urandom
 * 2.create a temptorary file with the random name to /tmp/ms_$NAME
 * 3. open writing and reading fd for the created file and use
 * unlink to make sure it it destroyed after closing all existing fds of it
 *
 * 4.check if the heredoc is actually used? /if there is anotherredirectin after
 * 	4.A if so normal read
 * 	4.B if not file is not needed and heredoc info is not actully saved anywhere
 * 5.read with readline check for termianting word and EOF and ctrl+c
 * 6.check if the line needs to be expanded or nor based on quotes in the termianting word
 * 7.
 * /
 *
 * bash: syntax error near unexpected token `>>' for missing redirections
 */

void	signal_handler_heredoc();
void	signal_handler_parent();

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void	remove_delim_quotes(char *delim)
{
	char	*temp;
	char	quote;

	quote = 0;
	temp = delim;
	while (*temp)
	{
		if (quote == *temp)
			quote = 0;
		else if (quote == 0 && (*temp == '\'' || *temp == '\"'))
			quote = *temp;
		else
		{
			*delim = *temp;
			delim++;
		}
		temp++;
	}
	*delim = 0;
}

int	ft_delim_expansion(char *delim)
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (delim[++i])
		if (delim[i] == '\'' || delim[i] == '"')
			ret = 1;
	if (ret == 1)
	{
		remove_delim_quotes(delim);
		return (1);
	}
	else
		return (0);
}

int	ft_heredoc_getline(t_io *io, char *delim, int fd_write, t_ms *ms)
{
	char	*line;
	int	expand;

	signal_handler_heredoc();
	expand = ft_delim_expansion(delim);
	printf("DELIM IS %s\n", delim);   //DELETE
	line = NULL;
	while (1)
	{
		line = readline(">");
		io->value = line;
		if (!line || ms_signal || !ft_strcmp(line ,delim))
			break;
		ms_signal = 0;   //NEEDED?
		if (expand == 1)
		{
			line = expand_argument(line, ms);   //need malloc check or happens insie exp_arg? maybe add line to io->value so it gets freed
			write(fd_write, line, ft_strlen(line));
			write(fd_write, "\n", 1);		//check write errors?
		}
		else
		{
			write(fd_write, line, ft_strlen(line));
			write(fd_write, "\n", 1);
		}
		free(line);
		line = NULL;
	}
	if (!line)
		printf("minishell: warning: here-document delimited by end of-file (wanted %s)\n", delim);
	if (ms_signal)
	{
		ms->exit_code = 130;
		ms->stop = 1;
	}
	free(line);
	io->value = NULL;
	signal_handler_parent();
	return(0);
}

int	create_random_name(char *name, char *temp)
{
	int		fd_urandom;
	int		read_bits;
	char		*base;
	unsigned char	bit;

	fd_urandom = open("/dev/urandom", O_RDONLY);
	if (fd_urandom == -1)
		return (1);
	read_bits = read(fd_urandom, temp, 20);
	close(fd_urandom);
	if (read_bits < 0)
		return (1);
	base = "abcdefghijklmnopqrstuvwxyz0123456789";
	while (read_bits--)
	{
		bit = *temp;
		*name = base[bit % ft_strlen(base)];
		temp++;
		name++;
	}
	*name = '\0';
	return (0);
}

int	ft_createfile(int *fd_write, int *fd_read)
{
	char	filename[64]; //if you change size, rememebr to change in other funcs also!
	char	gen_name[21]; //rand namepart will be 20 chars so 20+null
	char	temp[20];

	ft_strlcpy(filename, "/tmp/ms_", 64);
	if (create_random_name(gen_name, temp))
	{
		printf("Error creating random name\n");
		return(1);
	}
	ft_strlcat(filename, gen_name, 64);
	*fd_write = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
	*fd_read = open(filename, O_RDONLY);
	if (*fd_write == -1 || *fd_read == -1)
	{
		printf("Error opening heredoc file\n");
		if (*fd_write != -1)
			close(*fd_write);
		if (*fd_read != -1)
			close(*fd_read);
		return (1);
	}
	unlink(filename);
	return(0);
}

void	ft_empty_heredoc(t_ms *ms, t_io *io, char *delim)
{
	char *line;

	printf("EMPTY HEREDOC!\n");  //DEL
	signal_handler_heredoc();
	remove_delim_quotes(delim);
	printf("DELIM IS %s\n", delim);   //DEL
	while(1)
	{
		line = readline(">");
		if (!line || ms_signal || !ft_strcmp(line ,delim))
			break;
		free(line);
		line = NULL;
	}
	free(line);
	io->heredoc_fd = -1;
	if (ms_signal)
		ms->stop = 1;
	signal_handler_parent();
}

//takes t_io heredo node and adds the read fd to it.
int	ft_heredoc(t_ms *ms, t_io *io)
{
	int	fd_read;
	int	fd_write;
	//create random file name to /tmp/ms_{NAME}
	if (ft_createfile(&fd_write, &fd_read))
	{
		ms->stop = 1;
		return(1);
	}
	if (ft_heredoc_getline(io, io->value, fd_write, ms))
		close(fd_write);
	else
		io->heredoc_fd = fd_read;
	close(fd_write);
	return (0);
}

int	ft_heredoc_used(t_io *io)
{
	io = io->next;
	while (io)
	{
		if (io->type == T_HEREDOC || io->type == T_IN)
			return (1);
		io = io->next;
	}
	return(0);
}

int	ast_heredoc(t_ast *tree, t_ms *ms)
{
	t_io *io_temp;

	if (ms->stop || !tree)
		return (1);
	io_temp = tree->io_list;
	while (!ms->stop && tree->type == T_CMND && io_temp)
	{
		if (io_temp->type == T_HEREDOC)
		{
			if (ft_heredoc_used(io_temp))
				ft_empty_heredoc(ms, io_temp, io_temp->value);
			else
				ft_heredoc(ms, io_temp);
		}
		io_temp = io_temp->next;
	}
	if (tree->left && !ms->stop)
		ast_heredoc(tree->left, ms);
	if (tree->right && !ms->stop)
		ast_heredoc(tree->right, ms);
	return (0);
}
