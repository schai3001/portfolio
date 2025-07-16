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

static int	animation_collect(t_game *game)
{
	if (game->collect_frames == 14000)
		game->collect_frames = 0;
	put_collectible(game);
	game->collect_frames++;
	return (0);
}

static int	animation_enemy(t_game *game)
{
	if (game->enemy_frames == 2)
		game->enemy_frames = 0;
	put_enemy(game);
	game->enemy_frames++;
	return (0);
}

int	animation(t_game *game)
{
	animation_enemy(game);
	animation_collect(game);
	return (0);
}
