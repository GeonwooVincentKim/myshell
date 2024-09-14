/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hosokawa <hosokawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:34:01 by hosokawa          #+#    #+#             */
/*   Updated: 2024/09/14 13:19:02 by hosokawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "myshell.h"

//__attribute__((destructor)) static void destructor()
//{
//	system("leaks -q myshell");
//}

void exe_command(t_prompt_info *info)
{

	execve(str, &str, NULL)
}	

void	child_process(t_prompt_info *info)
{
	exe_command(info);
	error_set("cannot_exe_command",0,info);
}

	

void	parent_process(t_prompt_info *info, pid)
{
	int	recive_status;

	if (waitpid(pid, &recive_status, 0) == -1)
	{
		error_set("waitpid_error", 0, info);
		return ;
	}
	info->status = recive_status;
}

void	extern_command(t_prompt_info *info)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		error_warning("cannot_fork", 1, info);
	else if (pid == 0)
		chaild_process(info);
	else
		parent_process(info, pid);
}

void	prompt_operation(t_prompt_info *info)
{
	extern_command(info);
}

bool	shell_loop(t_prompt_info *info)
{
	info.str = readline("myshell:");
	if (str == NULL)
		return (FALSE);
	if (*(info.str))
	{
		add_history(*(info.str));
		prompt_operation(&info);
	}
	free(info.str);
	info.str = NULL;
	return TRUE
}

int	main(int argc, char **argv, char **envp)
{
	t_prompt_info	info;

	rl_outstream = stderr;
	info_init(&info, envp);
	while (1)
	{
		if (shell_loop(&info) != 1)
			break ;
	}
	clear_history();
	return (info.status);
}
