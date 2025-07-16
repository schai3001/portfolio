/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:20:47 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:53:48 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_texture_index(t_data *data, t_raydata *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir_x < 0)
			data->texturedata.index = 3;
		else
			data->texturedata.index = 2;
	}
	else
	{
		if (ray->dir_y > 0)
			data->texturedata.index = 1;
		else
			data->texturedata.index = 0;
	}
}

void	update_frame(t_data *data, t_texturedata *tex, t_raydata *ray, int x)
{
	int	y;
	int	color;

	get_texture_index(data, ray);
	tex->x = (int)(ray->wall_x * tex->size);
	if ((ray->side == 0 && ray->dir_x < 0)
		|| (ray->side == 1 && ray->dir_y > 0))
		tex->x = tex->size - tex->x - 1;
	tex->step = 1.0 * tex->size / ray->line_height;
	tex->pos = (ray->draw_start - data->win_height / 2
			+ ray->line_height / 2) * tex->step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex->y = (int)tex->pos & (tex->size - 1);
		tex->pos += tex->step;
		color = data->texture_tab[tex->index][tex->size * tex->y + tex->x];
		if (tex->index == 0 || tex->index == 2)
			color = (color >> 1) & 8355711;
		if (color > 0)
			data->texture_frame[y][x] = color;
		y++;
	}
}
