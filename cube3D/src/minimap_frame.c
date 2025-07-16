/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_frame.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:22 by schai             #+#    #+#             */
/*   Updated: 2024/01/23 13:45:47 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_minimap_pixels(t_minimapdata *minimap, int x, int y, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < minimap->square_size)
	{
		j = 0;
		while (j < minimap->square_size)
		{
			set_image_pixel(minimap->img, x + j, i + y, color);
			j++;
		}
		i++;
	}
}

static void	render_minimap_tile(t_minimapdata *minimap, int x, int y)
{
	if (minimap->map_tab[y][x] == 'P')
		set_minimap_pixels(minimap, x * minimap->square_size,
			y * minimap->square_size, MINIMAP_PLAYER);
	else if (minimap->map_tab[y][x] == '1')
		set_minimap_pixels(minimap, x * minimap->square_size,
			y * minimap->square_size, MINIMAP_WALL);
	else if (minimap->map_tab[y][x] == '0')
		set_minimap_pixels(minimap, x * minimap->square_size,
			y * minimap->square_size, MINIMAP_FLOOR);
	else if (minimap->map_tab[y][x] == ' ')
		set_minimap_pixels(minimap, x * minimap->square_size,
			y * minimap->square_size, MINIMAP_BLACK);
}

static void	render_borders(t_minimapdata *minimap, int color)
{
	int	size;
	int	x;
	int	y;

	size = MINIMAP_SQUARE + minimap->square_size;
	y = 0;
	while (y < size)
	{
		x = 0;
		while (x <= size)
		{
			if (x < 5 || x > size - 5 || y < 5 || y > size - 5)
				set_image_pixel(minimap->img, x, y, color);
			x++;
		}
		y++;
	}
}

static void	render_minimap(t_minimapdata *minimap)
{
	int	x;
	int	y;

	y = 0;
	while (y < minimap->size)
	{
		x = 0;
		while (x < minimap->size)
		{
			if (!minimap->map_tab[y] || !minimap->map_tab[y][x]
				|| minimap->map_tab[y][x] == '\0')
				break ;
			render_minimap_tile(minimap, x, y);
			x++;
		}
		y++;
	}
	render_borders(minimap, MINIMAP_BLACK);
}

void	minimap_frame(t_data *data, t_minimapdata *minimap)
{
	int	img_size;

	img_size = MINIMAP_SQUARE + minimap->square_size;
	init_new_img(data, &data->minimap, img_size, img_size);
	render_minimap(minimap);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.img,
		(data->win_width - (MINIMAP_SQUARE + minimap->square_size * 2)),
		(minimap->square_size * 2));
	mlx_destroy_image(data->mlx, data->minimap.img);
}
