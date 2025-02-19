/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:20:02 by strodrig          #+#    #+#             */
/*   Updated: 2025/01/27 12:20:02 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define EQUAL 0

# define RESET_COLOR "\x1B[0m"
# define CYAN "\x1B[36m"
# define MAGENTA "\x1B[35m"

typedef struct sigaction	t_sigaction;

typedef enum e_bool
{
	FALSE,
	TRUE,
}	t_bool;

enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	NEWLINE_TOKEN = '\n',
	QOUTES = '\'',
	DQUOTES = '\"',
	ESCAPE = '\\',
	ENV = '$',
	PIPE_TOKEN = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	HERE_DOC,
	DREDIR_OUT,
	_NULL = 0,
};

enum e_shell_state
{
	SH_READING,
	SH_EXECUTING,
};

typedef struct s_elem		t_elem;
typedef struct s_list		t_list;
typedef struct s_data		t_data;
typedef struct s_cmd		t_cmd;
typedef struct s_pipe		t_pipe;
typedef struct s_ast_node	t_ast_node;
typedef struct s_ast		t_ast;
typedef struct s_env_elem	t_env_elem;
typedef struct s_env		t_env;
typedef union u_union		t_union;

enum e_node_type
{
	CMD,
	PIPE,
};

typedef struct s_env_elem // Lista duplamente encadeada para armazenar as variáveis de ambiente como pares chave/valor.
{
	char				*key;	// Nome da variável de ambiente
	char				*value;	// Valor da variável de ambiente
	struct s_env_elem	*next;	// Próximo elemento na lista
	struct s_env_elem	*prev;	// Elemento anterior na lista
}	t_env_elem;

typedef struct s_env // Lista duplamente encadeada para armazenar as variáveis de ambiente.
{
	t_env_elem			*head;	// Cabeça da lista
	char				**env;	// Variáveis de ambiente
	int					size;	// Tamanho da lista
}	t_env;

enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	ESCAPED,
	GENERAL,
};

typedef struct s_cmd // Comando a ser executado
{
	char	**args;	// Argumentos do comando
	char	**env;	// Variáveis de ambiente
}	t_cmd;

typedef struct s_pipe // Estrutura para representar um pipe
{
	t_ast_node	*left;	// Nó esquerdo do pipe
	t_ast_node	*right;	// Nó direito do pipe
}	t_pipe;

typedef union u_union // União para armazenar diferentes tipos de conteúdo de nó
{
	t_pipe		*pipe;	// Conteúdo do tipo pipe
	t_cmd		*cmd;	// Conteúdo do tipo comando
}	t_union;

typedef struct s_ast_node // Nó da árvore de sintaxe abstrata (AST)
{
	enum e_node_type	type;		// Tipo do nó
	t_union				*content;	// Conteúdo do nó
}	t_ast_node;

typedef struct s_ast // Árvore de sintaxe abstrata (AST)
{
	t_ast_node	*root;	// Raiz da árvore
}	t_ast;

/*
O AST serio os comandos tipo ls -l | grep txt, o pipe seria o | e os comandos seriam ls -l e grep txt
*/

typedef struct s_elem // Elemento em uma lista duplamente encadeada
{
	char			*data;	// Dados do elemento
	int				len;	// Comprimento dos dados
	enum e_token	type;	// Tipo de token
	enum e_state	state;	// Estado do token
	t_elem			*next;	// Próximo elemento na lista
	t_elem			*prev;	// Elemento anterior na lista
}	t_elem;

typedef struct s_list // Lista duplamente encadeada
{
	t_elem	*head;	// Cabeça da lista
	t_elem	*tail;	// Cauda da lista
	int		size;	// Tamanho da lista
}	t_list;

typedef struct s_data // Dados gerais do MiniShell
{
	int		exit_status;	// Status de saída
	int		pid;			// ID do processo
	int		which;			// Indicador de qual comando
	int		alloc;			// Indicador de alocação
	char	**env;			// Variáveis de ambiente
	char	*siglist[NSIG];	// Lista de sinais
	t_list	*lex;			// Lista de tokens lexicais
	t_env	*ev;			// Lista de variáveis de ambiente
	t_ast	*tree;			// Árvore de sintaxe abstrata
	enum e_shell_state	shell_state;	// Estado do MiniShell
}	t_data;

/* --- minishell.c --- */
void	init_data(t_data *data);

/* --- signals.c --- */
void	init_signals(t_data *data);
void	signals_handler(int sig, siginfo_t *siginfo, void *ptr);
void	catch_sig(t_sigaction *sa);
void	mask_signals(t_sigaction *sa);
void	init_sig_list(t_data *data);

/* --- program.c --- */
int		cmd_count(t_ast_node *ptr);
void	ft_wait(void);
void	set_g_data(char **env);
int		ft_readline(char **line);
void	launch_shell(char **env);
void	execute_piped(char **cmds[], int num_cmds, char **envp);

/* --- utils.c --- */
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strlen(const char *s);
int		ft_strisspace(char *str);

/* --- exec.c --- */
char *find_executable(char *cmd);
void handle_redirections(char **args);
void execute_command(char **args, char **envp);
void execute_piped_commands(char **cmd1, char **cmd2, char **envp);

/* --- builtins.c --- */
void builtin_echo(char **args);
void builtin_cd(char **args);
void builtin_pwd();
void builtin_export(char **args);
void builtin_unset(char **args);
void builtin_env(char **envp);
void builtin_exit();
void execute_builtin(char **args, char **envp);
int is_builtin(char *cmd);

/* --- parser/ast.c --- */
t_ast	*init_tree(t_ast *tree);

/* --- env_var.c --- */
char *expand_variable(char *arg);
void expand_args(char **args);

extern t_data				*g_data_ptr;

#endif
