/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:22 by schai             #+#    #+#             */
/*   Updated: 2024/01/23 11:28:00 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_mouse_pos(t_data *data, int x, int y)
{
	if (x > data->win_width - MOUSE)
	{
		x = MOUSE;
		mlx_mouse_move(data->mlx, data->win, x, y);
	}
	if (x < MOUSE)
	{
		x = data->win_width - MOUSE;
		mlx_mouse_move(data->mlx, data->win, x, y);
	}
}

static int	mouse_input(int x, int y, t_data *data)
{
	static int	old_x = WIN_WIDTH / 2;

	get_mouse_pos(data, x, y);
	if (x == old_x)
		return (0);
	else if (x < old_x)
		data->playerdata.check_move += rotate_player(data, -1);
	else if (x > old_x)
		data->playerdata.check_move += rotate_player(data, 1);
	old_x = x;
	return (0);
}

static int	key_input(int key, t_data *data)
{
	if (key == XK_Escape)
		exit_cub(data);
	if (key == XK_Left)
		data->playerdata.rot -= 1;
	if (key == XK_Right)
		data->playerdata.rot += 1;
	if (key == XK_w)
		data->playerdata.move_y = 1;
	if (key == XK_a)
		data->playerdata.move_x = -1;
	if (key == XK_s)
		data->playerdata.move_y = -1;
	if (key == XK_d)
		data->playerdata.move_x = 1;
	return (0);
}

static int	key_output(int key, t_data *data)
{
	if (key == XK_Escape)
		exit_cub(data);
	if (key == XK_w && data->playerdata.move_y == 1)
		data->playerdata.move_y = 0;
	if (key == XK_s && data->playerdata.move_y == -1)
		data->playerdata.move_y = 0;
	if (key == XK_a && data->playerdata.move_x == -1)
		data->playerdata.move_x += 1;
	if (key == XK_d && data->playerdata.move_x == 1)
		data->playerdata.move_x -= 1;
	if (key == XK_Left && data->playerdata.rot <= 1)
		data->playerdata.rot = 0;
	if (key == XK_Right && data->playerdata.rot >= -1)
		data->playerdata.rot = 0;
	return (0);
}

void	input(t_data *data)
{
	mlx_hook(data->win, ClientMessage, NoEventMask, exit_cub, data);
	mlx_hook(data->win, KeyPress, KeyPressMask, key_input, data);
	mlx_hook(data->win, KeyRelease, KeyReleaseMask, key_output, data);
	mlx_hook(data->win, MotionNotify, PointerMotionMask,
		mouse_input, data);
}
