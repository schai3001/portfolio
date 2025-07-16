/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:20:47 by schai             #+#    #+#             */
/*   Updated: 2024/01/23 13:37:25 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_raycasting(int x, t_raydata *ray, t_playerdata *player)
{
	init_raydata(ray);
	ray->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir_x = player->dir_x + player->plane_x * ray->camera_x;
	ray->dir_y = player->dir_y + player->plane_y * ray->camera_x;
	ray->map_x = (int)player->pos_x;
	ray->map_y = (int)player->pos_y;
	ray->deltadist_x = fabs(1 / ray->dir_x);
	ray->deltadist_y = fabs(1 / ray->dir_y);
}

static void	get_line_height(t_raydata *ray, t_data *data, t_playerdata *player)
{
	if (ray->side == 0)
		ray->wall_dist = (ray->sidedist_x - ray->deltadist_x);
	else
		ray->wall_dist = (ray->sidedist_y - ray->deltadist_y);
	ray->line_height = (int)(data->win_height / ray->wall_dist);
	ray->draw_start = -(ray->line_height) / 2 + data->win_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + data->win_height / 2;
	if (ray->draw_end >= data->win_height)
		ray->draw_end = data->win_height - 1;
	if (ray->side == 0)
		ray->wall_x = player->pos_y + ray->wall_dist * ray->dir_y;
	else
		ray->wall_x = player->pos_x + ray->wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
}

int	raycasting(t_playerdata *playerdata, t_data *data)
{
	t_raydata	raydata;
	int			x;

	x = 0;
	raydata = data->raydata;
	while (x < data->win_width)
	{
		init_raycasting(x, &raydata, playerdata);
		init_dda(&raydata, playerdata);
		start_dda(data, &raydata);
		get_line_height(&raydata, data, playerdata);
		update_frame(data, &data->texturedata, &raydata, x);
		x++;
	}
	return (SUCCESS);
}

void	start_game(t_data *data)
{
	start_frame(data);
	start_minimap(data);
}

int	render(t_data *data)
{
	data->playerdata.check_move += move_player(data);
	if (data->playerdata.check_move == 0)
		return (0);
	start_game(data);
	return (0);
}
