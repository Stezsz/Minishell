/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:20:44 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/06 19:38:31 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/**
 * Builtins são comandos que são executados diretamente pelo shell e não por um processo filho.
 * Para isto tipo de comandos não vamos usar o execve, mas sim funções feitas por nós.
 * Os builtins que vamos implementar são:
 * - echo with option -n
 * - cd with only a relative or absolute path
 * - pwd
 * - export
 * - unset
 * - env
 * - exit
 */

/**
 * Imprime os argumentos passados para o comando echo.
 */
void builtin_echo(char **args)
{
	expand_args(args);
    for (int i = 1; args[i]; i++)
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
    }
    printf("\n");
}

/**
 * Muda o diretório de trabalho do shell.
 */
void builtin_cd(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "minishell: cd: missing argument\n");
        return;
    }
    if (chdir(args[1]) != 0)
        perror("cd");
}

/**
 * Imprime o diretório de trabalho atual.
 */
void builtin_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
}

/**
 * Adiciona ou modifica variáveis de ambiente.
 */
void builtin_export(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "minishell: export: missing argument\n");
        return;
    }
    if (putenv(args[1]) != 0)
        perror("export");
}

/**
 * Remove variáveis de ambiente.
 */
void builtin_unset(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "minishell: unset: missing argument\n");
        return;
    }
    if (unsetenv(args[1]) != 0)
        perror("unset");
}

/**
 * Imprime todas as variáveis de ambiente.
 */
void builtin_env(char **envp)
{
    for (int i = 0; envp[i]; i++)
        printf("%s\n", envp[i]);
}

/**
 * Termina o shell.
 */
void builtin_exit()
{
    printf("exit\n");
    exit(0);
}

/**
 * Verifica se o comando passado é um builtin.
 */
int is_builtin(char *cmd)
{
    return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 ||
            strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0 ||
            strcmp(cmd, "exit") == 0);
}

/**
 * Executa o builtin passado como argumento.
 */
void execute_builtin(char **args, char **envp)
{
    if (strcmp(args[0], "echo") == 0)
        builtin_echo(args);
    else if (strcmp(args[0], "cd") == 0)
        builtin_pwd();
    else if (strcmp(args[0], "pwd") == 0)
        builtin_cd(args);
    else if (strcmp(args[0], "export") == 0)
        builtin_export(args);
    else if (strcmp(args[0], "unset") == 0)
        builtin_unset(args);
    else if (strcmp(args[0], "env") == 0)
        builtin_env(envp);
    else if (strcmp(args[0], "exit") == 0)
        builtin_exit();
}
