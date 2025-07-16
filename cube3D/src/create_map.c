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

static int	get_map_lines(t_data *data, char **file, int i)
{
	int	start;
	int	j;

	start = i;
	while (file[i])
	{
		j = 0;
		while (file[i][j] == ' ' || file[i][j] == '\t' || file[i][j] == '\r'
		|| file[i][j] == '\v' || file[i][j] == '\f')
			j++;
		if (file[i][j] != '1')
			break ;
		i++;
	}
	data->mapdata.index_end_of_map = i;
	return (i - start);
}

static int	fill_map(t_mapdata *mapdata, char **map_tab, int index)
{
	int		i;
	int		j;

	mapdata->width = find_longest_line(mapdata, index);
	i = 0;
	while (i < mapdata->height)
	{
		j = 0;
		map_tab[i] = malloc(sizeof(char) * (mapdata->width + 1));
		if (!map_tab[i])
			return (error_msg(NULL, ERR_MALLOC, FAILURE));
		while (mapdata->file[index][j] && mapdata->file[index][j] != '\n')
		{
			map_tab[i][j] = mapdata->file[index][j];
			j++;
		}
		while (j < mapdata->width)
			map_tab[i][j++] = '\0';
		i++;
		index++;
	}
	map_tab[i] = NULL;
	return (SUCCESS);
}

static int	get_map(t_data *data, char **file, int i)
{
	data->mapdata.height = get_map_lines(data, file, i);
	data->map = malloc(sizeof(char *) * (data->mapdata.height + 1));
	if (!data->map)
		return (error_msg(NULL, ERR_MALLOC, FAILURE));
	if (fill_map(&data->mapdata, data->map, i) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static void	change_space_map(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j] == ' ' || data->map[i][j] == '\t'
		|| data->map[i][j] == '\r'
		|| data->map[i][j] == '\v' || data->map[i][j] == '\f')
			j++;
		while (data->map[i][++j])
		{
			if (data->map[i][j] == ' '
				&& j != data->map[i][ft_strlen(data->map[i]) - 1])
				data->map[i][j] = '1';
		}
		i++;
	}
}

int	create_map(t_data *data, char **file, int i)
{
	if (get_map(data, file, i) == FAILURE)
		return (FAILURE);
	if (check_vertical(data, data->map))
		return (FAILURE);
	if (check_horizontal(data->map))
		return (FAILURE);
	if (check_last_char(data->map) == FAILURE)
		return (FAILURE);
	if (vertical_check(data, data->map) == FAILURE)
		return (FAILURE);
	change_space_map(data);
	return (SUCCESS);
}
