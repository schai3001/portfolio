/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:07:37 by schai             #+#    #+#             */
/*   Updated: 2024/01/23 12:11:49 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_texturedata(t_texturedata *texturedata)
{
	texturedata->north = NULL;
	texturedata->south = NULL;
	texturedata->west = NULL;
	texturedata->east = NULL;
	texturedata->floor = 0;
	texturedata->ceiling = 0;
	texturedata->hex_floor = 0x0;
	texturedata->hex_ceiling = 0x0;
	texturedata->size = TEXTURE_SIZE;
	texturedata->step = 0.0;
	texturedata->pos = 0.0;
	texturedata->x = 0;
	texturedata->y = 0;
	texturedata->index = 0;
}

static void	init_mapdata(t_mapdata *mapdata)
{
	mapdata->fd = 0;
	mapdata->nb_line = 0;
	mapdata->path = NULL;
	mapdata->file = NULL;
	mapdata->height = 0;
	mapdata->width = 0;
	mapdata->index_end_of_map = 0;
}

static void	init_playerdata(t_playerdata *playerdata)
{
	playerdata->dir = '\0';
	playerdata->pos_x = 0.0;
	playerdata->pos_y = 0.0;
	playerdata->dir_x = 0.0;
	playerdata->dir_y = 0.0;
	playerdata->plane_x = 0.0;
	playerdata->plane_y = 0.0;
	playerdata->check_move = 0;
	playerdata->move_x = 0;
	playerdata->move_y = 0;
	playerdata->rot = 0;
}

void	init_raydata(t_raydata *raydata)
{
	raydata->camera_x = 0;
	raydata->dir_x = 0;
	raydata->dir_y = 0;
	raydata->map_x = 0;
	raydata->map_y = 0;
	raydata->step_x = 0;
	raydata->step_y = 0;
	raydata->sidedist_x = 0;
	raydata->sidedist_y = 0;
	raydata->deltadist_x = 0;
	raydata->deltadist_y = 0;
	raydata->wall_dist = 0;
	raydata->wall_x = 0;
	raydata->side = 0;
	raydata->line_height = 0;
	raydata->draw_start = 0;
	raydata->draw_end = 0;
}

void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->win_height = WIN_HEIGHT;
	data->win_width = WIN_WIDTH;
	data->map = NULL;
	init_mapdata(&data->mapdata);
	init_texturedata(&data->texturedata);
	init_playerdata(&data->playerdata);
	data->texture_tab = NULL;
	data->texture_frame = NULL;
	init_imgdata(&data->minimap);
}
