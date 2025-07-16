/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:47:46 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:23:14 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_texture_frame(t_data *data)
{
	int	i;

	if (data->texture_frame)
		free_tab((void **)data->texture_frame);
	data->texture_frame = ft_calloc(data->win_height + 1,
			sizeof * data->texture_frame);
	if (!data->texture_frame)
		clean_exit(data, error_msg(NULL, ERR_MALLOC, 1));
	i = 0;
	while (i < data->win_height)
	{
		data->texture_frame[i] = ft_calloc(data->win_width + 1,
				sizeof * data->texture_frame);
		if (!data->texture_frame[i])
			clean_exit(data, error_msg(NULL, ERR_MALLOC, 1));
		i++;
	}
}

void	render_frame(t_data *data)
{
	t_imgdata	image;
	int			x;
	int			y;

	image.img = NULL;
	init_new_img(data, &image, data->win_width, data->win_height);
	y = 0;
	while (y < data->win_height)
	{
		x = 0;
		while (x < data->win_width)
		{
			set_frame(data, &image, x, y);
			x++;
		}		
		y++;
	}
	mlx_put_image_to_window(data->mlx, data->win, image.img, 0, 0);
	mlx_destroy_image(data->mlx, image.img);
}

void	set_pixel(t_imgdata *image, int x, int y, int color)
{
	int	pixel;

	pixel = y * (image->size_line / 4) + x;
	image->address[pixel] = color;
}

void	set_frame(t_data *data, t_imgdata *image, int x, int y)
{
	if (data->texture_frame[y][x] > 0)
		set_pixel(image, x, y, data->texture_frame[y][x]);
	else if (y < data->win_height / 2)
		set_pixel(image, x, y, data->texturedata.hex_ceiling);
	else if (y < data->win_height -1)
		set_pixel(image, x, y, data->texturedata.hex_floor);
}

void	start_frame(t_data *data)
{
	init_texture_frame(data);
	init_raydata(&data->raydata);
	raycasting(&data->playerdata, data);
	render_frame(data);
}
