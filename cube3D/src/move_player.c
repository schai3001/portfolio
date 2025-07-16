/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:20:47 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:13:12 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	move_forward(t_data *data)
{
	double	new_x;
	double	new_y;

	new_x = data->playerdata.pos_x + data->playerdata.dir_x * MS;
	new_y = data->playerdata.pos_y + data->playerdata.dir_y * MS;
	return (check_player_position(data, new_x, new_y));
}

static int	move_backward(t_data *data)
{
	double	new_x;
	double	new_y;

	new_x = data->playerdata.pos_x - data->playerdata.dir_x * MS;
	new_y = data->playerdata.pos_y - data->playerdata.dir_y * MS;
	return (check_player_position(data, new_x, new_y));
}

static int	move_left(t_data *data)
{
	double	new_x;
	double	new_y;

	new_x = data->playerdata.pos_x + data->playerdata.dir_y * MS;
	new_y = data->playerdata.pos_y - data->playerdata.dir_x * MS;
	return (check_player_position(data, new_x, new_y));
}

static int	move_right(t_data *data)
{
	double	new_x;
	double	new_y;

	new_x = data->playerdata.pos_x - data->playerdata.dir_y * MS;
	new_y = data->playerdata.pos_y + data->playerdata.dir_x * MS;
	return (check_player_position(data, new_x, new_y));
}

int	move_player(t_data *data)
{
	int	i;

	i = 0;
	if (data->playerdata.move_y == 1)
		i += move_forward(data);
	if (data->playerdata.move_y == -1)
		i += move_backward(data);
	if (data->playerdata.move_x == -1)
		i += move_left(data);
	if (data->playerdata.move_x == 1)
		i += move_right(data);
	if (data->playerdata.rot != 0)
		i += rotate_player(data, data->playerdata.rot);
	return (i);
}
