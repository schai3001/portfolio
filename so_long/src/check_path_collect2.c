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

static int	is_path_c(int **map_int, t_ull i, t_ull j, int **map_visited)
{
	if (map_visited[i][j] == 0 && map_int[i][j] != 1)
	{
		map_visited[i][j] = 1;
		if (map_int[i][j] == 2)
		{
			map_int[i][j] = 0;
			return (1);
		}
		else if (is_path_c(map_int, i - 1, j, map_visited))
			return (1);
		else if (is_path_c(map_int, i, j - 1, map_visited))
			return (1);
		else if (is_path_c(map_int, i + 1, j, map_visited))
			return (1);
		else if (is_path_c(map_int, i, j + 1, map_visited))
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

void	create_map_int2(int **map_int, t_game *game)
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
				map_int[i][j] = 2;
			if (game->map[i][j] == 'P')
				map_int[i][j] = 3;
			if (game->map[i][j] == 'E')
				map_int[i][j] = 0;
			if (game->map[i][j] == '0')
				map_int[i][j] = 0;
			if (game->map[i][j] == '1')
				map_int[i][j] = 1;
			j++;
		}
		i++;
	}
}

void	create_map_visited2(int **map_visited, int **map_int, t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (i < game->heigth)
	{
		j = 0;
		while (j < game->width)
		{
			map_visited[i][j] = 0;
			if (map_int[i][j] == 1)
				map_visited[i][j] = 1;
			j++;
		}
		i++;
	}
}

void	reset_visited(t_game *game, int **map_visited, int **map_int)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (i < game->heigth)
	{
		j = 0;
		while (j < game->width)
		{
			map_visited[i][j] = 0;
			if (map_int[i][j] == 1)
				map_visited[i][j] = 1;
			j++;
		}
		i++;
	}
}

void	check_path_collect2(t_game *game, int **map_int, int **map_visited)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (i < game->heigth)
	{
		j = 0;
		while (j < game->width)
		{
			if (map_int[i][j] == 3 && map_visited[i][j] == 0)
			{
				while (is_path_c(map_int, i, j, map_visited))
				{
					reset_visited(game, map_visited, map_int);
					game->count_collectible++;
				}
			}
			j++;
		}
		i++;
	}
}
