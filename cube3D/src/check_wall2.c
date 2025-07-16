/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_wall2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/02/06 14:12:06 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	base_check(char **map_tab, int x, int y)
{
	if (y == 0 && map_tab[y + 1] == NULL)
		return (FAILURE);
	else if (y == 0 && map_tab[y + 1][x] == '\0')
		return (SUCCESS);
	else if (y == 0 && !check_char(map_tab[y][x], "1 "))
		return (FAILURE);
	else if (y == 0 && x == 0 && map_tab[y][x] == '0')
		return (FAILURE);
	else
		return (SUCCESS);
}

static int	check_column(t_data *data, char **map_tab, int y, int x)
{
	while (y < data->mapdata.height && check_char(map_tab[y][x], "10NSEW "))
	{
		base_check(map_tab, x, y);
		if (map_tab[y][x] == ' ')
		{
			if (y > 0 && map_tab[y - 1][x] != '1')
				return (FAILURE);
			while (y < data->mapdata.height && map_tab[y][x] == ' ')
				y++;
			if (y < data->mapdata.height && map_tab[y][x] == '\0')
				return (SUCCESS);
			if (y < data->mapdata.height && map_tab[y][x] != '1')
				return (FAILURE);
		}
		if (y < data->mapdata.height)
			y++;
	}
	if (y > 0 && !check_char(map_tab[y - 1][x], "1 "))
		return (FAILURE);
	return (SUCCESS);
}

int	check_line(char *line, int x)
{
	while (line[x])
	{
		if (x == 0 && !check_char(line[x], "1 "))
			return (FAILURE);
		if (!check_char(line[x], "10NSEW "))
			return (FAILURE);
		if (line[x] == ' ')
		{
			if (x > 0 && line[x - 1] != '1')
				return (FAILURE);
			while (line[x] && line[x] == ' ')
				x++;
			if (line[x] && line[x] != '1')
				return (FAILURE);
		}
		if (line[x])
			x++;
	}
	if (!check_char(line[x - 1], "1 "))
		return (FAILURE);
	return (SUCCESS);
}

int	check_vertical(t_data *data, char **map_tab)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (x < data->mapdata.width)
	{
		if (check_column(data, map_tab, y, x) == FAILURE)
			return (FAILURE);
		x++;
	}
	return (SUCCESS);
}

int	check_horizontal(char **map_tab)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (map_tab[y])
	{
		if (check_line(map_tab[y], x) == FAILURE)
			return (FAILURE);
		y++;
	}
	return (SUCCESS);
}
