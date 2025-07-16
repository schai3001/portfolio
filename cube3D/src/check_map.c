/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/01/02 15:03:13 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_map_elem(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	i = 0;
	data->playerdata.dir = '0';
	while (map_tab[i] != NULL)
	{
		j = 0;
		while (map_tab[i][j])
		{
			while (data->map[i][j] == ' ' || data->map[i][j] == '\t'
			|| data->map[i][j] == '\r'
			|| data->map[i][j] == '\v' || data->map[i][j] == '\f')
				j++;
			if (!(ft_strchr("10NSEW", map_tab[i][j])))
				return (error_msg(NULL, WRONG_CHAR, FAILURE));
			if (ft_strchr("NSEW", map_tab[i][j]) && data->playerdata.dir != '0')
				return (error_msg(NULL, WRONG_CHAR, FAILURE));
			if (ft_strchr("NSEW", map_tab[i][j]) && data->playerdata.dir == '0')
				data->playerdata.dir = map_tab[i][j];
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

int	check_position(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	i = (int)data->playerdata.pos_y;
	j = (int)data->playerdata.pos_x;
	if (ft_strlen(map_tab[i - 1]) < (size_t)j
		|| ft_strlen(map_tab[i + 1]) < (size_t)j
		|| check_is_whitespace(map_tab[i][j - 1]) == SUCCESS
		|| check_is_whitespace(map_tab[i][j + 1]) == SUCCESS
		|| check_is_whitespace(map_tab[i - 1][j]) == SUCCESS
		|| check_is_whitespace(map_tab[i + 1][j]) == SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}

int	check_player_pos(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	if (data->playerdata.dir == '0')
		return (error_msg(NULL, WRONG_CHAR, FAILURE));
	i = 0;
	while (map_tab[i])
	{
		j = 0;
		while (map_tab[i][j])
		{
			if (ft_strchr("NSEW", map_tab[i][j]))
			{
				data->playerdata.pos_x = (double)j + 0.5;
				data->playerdata.pos_y = (double)i + 0.5;
				map_tab[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	if (check_position(data, map_tab) == FAILURE)
		return (error_msg(NULL, INV_PLAYER_POS, FAILURE));
	return (SUCCESS);
}

int	check_map_is_at_the_end(t_mapdata *map)
{
	int	i;
	int	j;

	i = map->index_end_of_map;
	while (map->file[i])
	{
		j = 0;
		while (map->file[i][j])
		{
			if (map->file[i][j] != ' ' && map->file[i][j] != '\t'
				&& map->file[i][j] != '\r' && map->file[i][j] != '\n'
				&& map->file[i][j] != '\v' && map->file[i][j] != '\f')
				return (FAILURE);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

int	check_map(t_data *data, char **map_tab)
{
	if (!data->map)
		return (error_msg(NULL, MAP_MISSING, FAILURE));
	if (data->mapdata.height < 3)
		return (error_msg(NULL, MAP_TOO_SMALL, FAILURE));
	if (check_map_walls(&data->mapdata, map_tab) == FAILURE)
		return (error_msg(NULL, MAP_NO_WALLS, FAILURE));
	if (check_map_elem(data, map_tab) == FAILURE)
		return (FAILURE);
	if (check_player_pos(data, map_tab) == FAILURE)
		return (FAILURE);
	if (check_map_is_at_the_end(&data->mapdata) == FAILURE)
		return (error_msg(NULL, MAP_LAST_ELEM, FAILURE));
	return (SUCCESS);
}
