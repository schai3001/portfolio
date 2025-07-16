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

void	check_game_size(t_game *game)
{
	t_ull	i;

	i = 0;
	while (game->map[i])
		i++;
	game->width = ft_strlen(game->map[0]);
	game->heigth = i;
	if (game->width < 3 || game->heigth < 3)
	{
		free_game(game);
		exit_with_error("Error with Map Size !\nExit Program ");
	}
	i = 0;
	while (game->map[i])
	{
		if (ft_strlen(game->map[i]) != game->width)
		{
			free_game(game);
			exit_with_error("Error in Map Form !\nExit Program ");
		}
		i++;
	}
}

void	check_map2(t_game *game)
{
	check_game_size(game);
	check_walls_h(game);
	check_walls_h2(game);
	check_walls_v(game);
	check_walls_v2(game);
	game->start = count_player(game);
	game->exit = count_exit(game);
	if (game->start != 1)
	{
		free_game(game);
		exit_with_error("Error with player !\nExit Program ");
	}
	if (game->exit != 1)
	{
		free_game(game);
		exit_with_error("Error with exit !\nExit Program ");
	}
}
