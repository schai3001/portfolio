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

void	check_walls_v2(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		len;

	i = 0;
	j = 0;
	len = ft_strlen(game->map[0]);
	while (game->map[i])
		i++;
	while (j < i)
	{
		if (game->map[j][len - 1] != '1')
		{
			free_game(game);
			exit_with_error("Error in Walls !\nExit Program ");
		}
		j++;
	}
}

void	check_walls_v(t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	j = 0;
	while (game->map[i])
		i++;
	while (j < i)
	{
		if (game->map[j][0] != '1')
		{
			free_game(game);
			exit_with_error("Error in Walls !\nExit Program ");
		}
		j++;
	}
}

void	check_walls_h2(t_game *game)
{
	t_ull	i;

	i = 0;
	while (game->map[0][i])
	{
		if (game->map[0][i] != '1')
		{
			free_game(game);
			exit_with_error("Error in Walls !\nExit Program ");
		}
		i++;
	}
}

void	check_walls_h(t_game *game)
{
	t_ull	i;
	t_ull	j;

	j = 0;
	i = 0;
	while (game->map[i])
		i++;
	while (game->map[i - 1][j])
	{
		if (game->map[i - 1][j] != '1')
		{
			free_game(game);
			exit_with_error("Error in Walls !\nExit Program ");
		}
		j++;
	}
}

void	check_map(t_game *game)
{
	check_map2(game);
	game->collectible = count_collect(game);
	if (game->collectible == 0)
	{
		free_game(game);
		exit_with_error("Error with Collectible !\nExit Program ");
	}
	game->enemy = count_enemy(game);
	if (count_parasite(game) != 0)
	{
		free_game(game);
		exit_with_error("Error with char(s) in map !\nExit Program ");
	}
	check_path_collect(game);
	check_path_exit(game);
}
