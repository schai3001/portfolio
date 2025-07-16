/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:37:25 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_rgb(int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (rgb[i] < 0 || rgb[i] > 255)
			return (error_msg(NULL, INV_RGB, FAILURE));
		i++;
	}
	return (SUCCESS);
}

static unsigned long	convert_rgb_to_hex(int *rgb)
{
	unsigned long	hex_result;
	int				r;
	int				g;
	int				b;

	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	hex_result = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	return (hex_result);
}

int	check_textures(t_texturedata *textures)
{
	if (!textures->north || !textures->south || !textures->west
		|| !textures->east)
		return (error_msg(NULL, MISS_TEXTURE, FAILURE));
	if (!textures->floor || !textures->ceiling)
		return (error_msg(NULL, MISS_COLOR, FAILURE));
	if (check_file(textures->north, false) == FAILURE
		|| check_file(textures->south, false) == FAILURE
		|| check_file(textures->west, false) == FAILURE
		|| check_file(textures->east, false) == FAILURE
		|| check_rgb(textures->floor) == FAILURE
		|| check_rgb(textures->ceiling) == FAILURE)
		return (FAILURE);
	textures->hex_ceiling = convert_rgb_to_hex(textures->ceiling);
	textures->hex_floor = convert_rgb_to_hex(textures->floor);
	return (SUCCESS);
}
