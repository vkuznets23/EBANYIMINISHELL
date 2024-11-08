# Name of the executable
NAME = minishell

# Compiler and flags
CC = gcc
#CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include -I./includes -I./Libft
#CFLAGS = -Wall -Wextra -Werror $(CPPFLAGS)  # Include CPPFLAGS for compilation
#LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline  # Include LDFLAGS for linking
CFLAGS = -Wall -Wextra -Werror -I/usr/local/opt/readline/include -I./includes -I./Libft
LDFLAGS = -L/usr/local/opt/readline/lib -lreadline

# Directories
LIBFT_DIR = ./Libft
INCLUDES_DIR = ./includes
PARSING_DIR = ./parsing
TOKEN_DIR = ./token
EXP_DIR = ./expansion
EXEC_DIR = ./execution
BLT_DIR = ./buildins
UTL_DIR = ./utils

# Library files
LIBFT = $(LIBFT_DIR)/libft.a

PARSING_SRCS = \
    $(PARSING_DIR)/parsing.c \
    $(PARSING_DIR)/parsing_error_init.c \
    $(PARSING_DIR)/parsing_node.c \
    $(PARSING_DIR)/heredoc.c \
    $(PARSING_DIR)/heredoc_file.c \
    $(PARSING_DIR)/heredoc_read.c \
    $(PARSING_DIR)/clean_ast.c \
    $(PARSING_DIR)/parsing_utils.c

TOKEN_SRCS = \
    $(TOKEN_DIR)/print_ast.c \
    $(TOKEN_DIR)/token.c \
    $(TOKEN_DIR)/token_create.c \
    $(TOKEN_DIR)/token_utils.c \
    $(TOKEN_DIR)/main.c \
    ./signal/signal.c

EXPANSION_SRCS = \
    $(EXP_DIR)/expansion.c \
    $(EXP_DIR)/expansion_cases.c \
    $(EXP_DIR)/expansion_utils.c

EXECUTION_SRCS = \
    $(EXEC_DIR)/envp.c \
    $(EXEC_DIR)/exec_buildin.c \
    $(EXEC_DIR)/find_executable.c \
    $(EXEC_DIR)/redirection.c \
    $(EXEC_DIR)/execution.c \
    $(EXEC_DIR)/redirection_parent.c \
    $(EXEC_DIR)/execution_utils.c

BUILTIN_SRCS = \
    $(BLT_DIR)/builtin_export.c \
    $(BLT_DIR)/builtin_exit.c \
    $(BLT_DIR)/builtin_env.c \
    $(BLT_DIR)/builtin_echo.c \
    $(BLT_DIR)/builtin_cd.c \
    $(BLT_DIR)/builtin_unset.c

UTILS_SRCS = \
    $(UTL_DIR)/clean.c

# Combined source files
OBJS = $(PARSING_SRCS) $(TOKEN_SRCS) $(EXPANSION_SRCS) $(EXECUTION_SRCS) $(BUILTIN_SRCS) $(UTILS_SRCS)

CLEAN = $(OBJS:.c=.o)

# Includes and headers
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

# Rules

# Default target
all: $(LIBFT) $(NAME)

# Build minishell executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build the libft library
$(LIBFT):
	make -C $(LIBFT_DIR)

# Clean object files
clean:
	rm -f $(CLEAN)
	make -C $(LIBFT_DIR) clean

# Clean object files and the executable
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re

