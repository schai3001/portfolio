/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_walls.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/02/06 12:27:57 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_wall(char **map_tab, int i, int j)
{
	if (!map_tab || !map_tab[i] || !map_tab[i][j])
		return (FAILURE);
	while (map_tab[i][j] == ' ' || map_tab[i][j] == '\t'
	|| map_tab[i][j] == '\r' || map_tab[i][j] == '\v'
	|| map_tab[i][j] == '\f')
		j++;
	while (map_tab[i][j] != '\0')
	{
		if (!check_char(map_tab[i][j], "1 "))
			return (FAILURE);
		j++;
	}
	return (SUCCESS);
}

int	vertical_check(t_data *data, char **map_tab)
{
	int	y;
	int	x;

	x = 0;
	y = 1;
	while (y < data->mapdata.height - 1)
	{
		x = 1;
		while (map_tab[y][x])
		{
			if (map_tab[y][x] == '0')
			{
				if (map_tab[y - 1][x] == '\0' || map_tab[y + 1][x] == '\0')
					return (FAILURE);
				if (map_tab[y - 1][x] == ' ' || map_tab[y + 1][x] == ' ')
					return (FAILURE);
			}
			x++;
		}
		y++;
	}
	return (SUCCESS);
}

int	check_map_walls(t_mapdata *mapdata, char **map_tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (check_wall(map_tab, 0, 0) == FAILURE)
		return (FAILURE);
	i = 1;
	while (i < (mapdata->height - 1))
	{
		j = ft_strlen(map_tab[i]) - 1;
		if (map_tab[i][j] != '1')
			return (FAILURE);
		i++;
	}
	if (check_wall(map_tab, i, 0) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
