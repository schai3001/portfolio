/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 19:06:38 by mcombeau          #+#    #+#             */
/*   Updated: 2023/10/10 17:43:40 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_shell(t_data *data, int exno)
{
	if (data)
	{
		if (data->cmd && data->cmd->io_fds)
			close_fds(data->cmd, true);
		free_data(data, true);
	}
	exit(exno);
}
