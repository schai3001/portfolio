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

static void	free_map(int **map, t_game *game)
{
	t_ull	i;

	i = 0;
	while (i < game->heigth && map[i] != 0)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

static int	**create_map_int(t_game *game)
{
	t_ull	i;
	int		**map_int;

	i = 0;
	map_int = (int **)malloc(sizeof(int *) * (game->heigth));
	if (!map_int)
		return (NULL);
	while (i < game->heigth)
	{
		map_int[i] = (int *)malloc(sizeof(int) * (game->width));
		if (!map_int[i])
		{
			free_map(map_int, game);
			return (NULL);
		}
		i++;
	}
	create_map_int2(map_int, game);
	return (map_int);
}

static int	**create_map_visited(t_game *game, int **map_int)
{
	int		**map_visited;
	t_ull	i;

	i = 0;
	map_visited = (int **)malloc(sizeof(int *) * (game->heigth));
	if (!map_visited)
	{
		free_map(map_int, game);
		return (NULL);
	}
	while (i < game->heigth)
	{
		map_visited[i] = (int *)malloc(sizeof(int) * (game->width));
		if (!map_visited[i])
		{
			free_map(map_int, game);
			free_map(map_visited, game);
			return (NULL);
		}
		i++;
	}
	create_map_visited2(map_visited, map_int, game);
	return (map_visited);
}

void	check_path_collect(t_game *game)
{
	int		**map_int;
	int		**map_visited;

	map_int = create_map_int(game);
	if (!map_int)
	{
		free_game(game);
		exit_with_error("Error with Memory !\nExit Program ");
	}
	map_visited = create_map_visited(game, map_int);
	if (!map_visited)
	{
		free_game(game);
		exit_with_error("Error with Memory !\nExit Program ");
	}
	check_path_collect2(game, map_int, map_visited);
	free_map(map_int, game);
	free_map(map_visited, game);
	if (game->count_collectible != game->collectible)
	{
		free_game(game);
		exit_with_error("Error with Collectible Path !\nExit Program ");
	}
}
