/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/02/05 14:42:18 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_image_pixel(t_imgdata *image, int x, int y, int color)
{
	int	pixel;

	pixel = y * (image->size_line / 4) + x;
	image->address[pixel] = color;
}

size_t	find_longest_line(t_mapdata *map, int i)
{
	size_t	biggest_len;

	biggest_len = ft_strlen(map->file[i]);
	while (map->file[i])
	{
		if (ft_strlen(map->file[i]) > biggest_len)
			biggest_len = ft_strlen(map->file[i]);
		i++;
	}
	return (biggest_len);
}

int	check_is_whitespace(char c)
{
	if (c != ' ' && c != '\t' && c != '\r'
		&& c != '\n' && c != '\v' && c != '\f')
		return (FAILURE);
	else
		return (SUCCESS);
}

int	check_char(char c, char *valid_char)
{
	int	i;

	i = 0;
	while (valid_char[i])
	{
		if (c == valid_char[i])
			return (1);
		i++;
	}
	return (0);
}

int	check_last_char(char **map_tab)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (map_tab[y])
	{
		x = ft_strlen(map_tab[y]) - 1;
		while (map_tab[y][x] == ' ')
		{
			x--;
		}
		if (map_tab[y][x] != '1')
			return (FAILURE);
		y++;
	}
	return (SUCCESS);
}
