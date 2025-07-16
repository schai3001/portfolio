/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 18:54:19 by schai             #+#    #+#             */
/*   Updated: 2022/12/28 18:54:27 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	is_path_e(int **map_int, t_ull i, t_ull j, int **map_visited)
{
	if (map_visited[i][j] == 0 && map_int[i][j] != 1)
	{
		map_visited[i][j] = 1;
		if (map_int[i][j] == 2)
			return (1);
		else if (is_path_e(map_int, i - 1, j, map_visited))
			return (1);
		else if (is_path_e(map_int, i, j - 1, map_visited))
			return (1);
		else if (is_path_e(map_int, i + 1, j, map_visited))
			return (1);
		else if (is_path_e(map_int, i, j + 1, map_visited))
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

void	create_map_int2_e(int **map_int, t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (i < game->heigth)
	{
		j = 0;
		while (j < game->width)
		{
			if (game->map[i][j] == 'K')
				map_int[i][j] = 1;
			if (game->map[i][j] == 'C')
				map_int[i][j] = 0;
			if (game->map[i][j] == 'P')
				map_int[i][j] = 3;
			if (game->map[i][j] == 'E')
				map_int[i][j] = 2;
			if (game->map[i][j] == '0')
				map_int[i][j] = 0;
			if (game->map[i][j] == '1')
				map_int[i][j] = 1;
			j++;
		}
		i++;
	}
}
