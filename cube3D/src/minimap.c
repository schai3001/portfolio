/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:22 by schai             #+#    #+#             */
/*   Updated: 2024/01/23 13:46:37 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	check_pos_minimap(int pos, int size)
{
	if (pos < size)
		return (true);
	return (false);
}

static char	*add_map_line(t_data *data, t_minimapdata *minimap, int y)
{
	char	*line;
	int		x;

	line = ft_calloc(minimap->size + 1, sizeof * line);
	if (!line)
		return (NULL);
	x = 0;
	while (x < minimap->size && x < data->mapdata.width)
	{
		if (!check_pos_minimap(y + minimap->y, data->mapdata.height)
			|| !check_pos_minimap(x + minimap->x, data->mapdata.width))
			line[x] = '\0';
		else if ((int)data->playerdata.pos_x == (x + minimap->x)
			&& (int)data->playerdata.pos_y == (y + minimap->y))
			line[x] = 'P';
		else if (data->map[y + minimap->y][x + minimap->x] == '1')
			line[x] = '1';
		else if (data->map[y + minimap->y][x + minimap->x] == '0')
			line[x] = '0';
		else
			line[x] = '\0';
		x++;
	}
	return (line);
}

static char	**init_minimap_tab(t_data *data, t_minimapdata *minimap)
{
	char	**map;
	int		y;

	map = ft_calloc(minimap->size + 1, sizeof * map);
	if (!map)
		return (NULL);
	y = 0;
	while (y < minimap->size && y < data->mapdata.height)
	{
		map[y] = add_map_line(data, minimap, y);
		if (!map[y])
		{
			free_tab((void **)map);
			return (NULL);
		}
		y++;
	}
	return (map);
}

static int	get_minimap_xy(t_minimapdata *minimap, int mapsize, int pos)
{
	if (pos > minimap->distance && mapsize - pos > minimap->distance + 1)
		return (pos - minimap->distance);
	if (pos > minimap->distance && mapsize - pos <= minimap->distance + 1)
		return (mapsize - minimap->size);
	return (0);
}

void	start_minimap(t_data *data)
{
	t_minimapdata	minimap;

	minimap.map_tab = NULL;
	minimap.img = &data->minimap;
	minimap.distance = MINIMAP_DISTANCE;
	minimap.size = (2 * minimap.distance) + 1;
	minimap.square_size = MINIMAP_SQUARE / (2 * minimap.distance);
	minimap.x = get_minimap_xy(&minimap, data->mapdata.width,
			(int)data->playerdata.pos_x);
	minimap.y = get_minimap_xy(&minimap, data->mapdata.height,
			(int)data->playerdata.pos_y);
	minimap.map_tab = init_minimap_tab(data, &minimap);
	if (!minimap.map_tab)
	{
		error_msg(NULL, ERR_MALLOC, 0);
		return ;
	}
	minimap_frame(data, &minimap);
	free_tab((void **)minimap.map_tab);
}
