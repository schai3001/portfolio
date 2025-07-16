/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_position.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:12:23 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	check_new_position(t_data *data, double x, double y)
{
	if (data->map[(int)y][(int)x] == '0')
		return (true);
	return (false);
}

int	check_player_position(t_data *data, double new_x, double new_y)
{
	int	i;

	i = 0;
	if (check_new_position(data, new_x, data->playerdata.pos_y))
	{
		data->playerdata.pos_x = new_x;
		i = 1;
	}
	if (check_new_position(data, data->playerdata.pos_x, new_y))
	{
		data->playerdata.pos_y = new_y;
		i = 1;
	}
	return (i);
}
