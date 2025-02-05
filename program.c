/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 10:50:25 by strodrig          #+#    #+#             */
/*   Updated: 2025/02/04 10:50:25 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para escrever o prompt do shell
void	write_prompt(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0); // Obter o diretório de trabalho atual
	if (!cwd)
		return;
	write(1, CYAN, 5);
	write(1, cwd, strlen(cwd));
	write(1, MAGENTA, 5);
	write(1, "$ ", 2);
	write(1, RESET_COLOR, 4);
	free(cwd);
}

// Função para ler uma linha da entrada do usuário
int	ft_readline(char **line)
{
	write_prompt();
	*line = readline("");
	if (!*line)
	{
		write(1, "exit\n", 5);
		exit(1);
	}
	if (ft_strcmp(*line, "") == EQUAL || ft_strisspace(*line))
	{
		free(*line);
		return (1);
	}
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (0);
}

// Função para definir dados globais (atualmente não faz nada)
void	set_g_data(char **env)
{
	(void)env;
}

// Função para iniciar o shell
void	launch_shell(char **env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		set_g_data(env); // Definir dados globais
		if (ft_readline(&line))
			continue ;
		// ...existing code...
	}
}
