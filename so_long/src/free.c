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
#include <stdlib.h>

static void	free_img_player(t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 2)
		{
			if (game->img_player[i][j].img_ptr)
			{
				mlx_destroy_image(game->mlx_ptr,
					game->img_player[i][j].img_ptr);
			}
			j++;
		}
		i++;
	}
}

static void	free_img2(t_game *game)
{
	if (game->img_tile.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->img_tile.img_ptr);
	if (game->img_exit.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->img_exit.img_ptr);
	if (game->img_victory.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->img_victory.img_ptr);
	if (game->img_defeat.img_ptr)
		mlx_destroy_image(game->mlx_ptr, game->img_defeat.img_ptr);
}

static void	free_img(t_game *game)
{
	t_ull	i;

	i = 0;
	while (i < 7)
	{
		if (game->img_collect[i][0].img_ptr)
			mlx_destroy_image(game->mlx_ptr, game->img_collect[i][0].img_ptr);
		i++;
	}
	i = 0;
	while (i < 8)
	{
		if (game->img_wall[i][0].img_ptr)
			mlx_destroy_image(game->mlx_ptr, game->img_wall[i][0].img_ptr);
		i++;
	}
	i = 0;
	while (i < 2)
	{
		if (game->img_enemy[i][0].img_ptr)
			mlx_destroy_image(game->mlx_ptr, game->img_enemy[i][0].img_ptr);
		i++;
	}
}

void	free_game(t_game *game)
{
	t_ull	i;

	i = 0;
	while (game->map[i])
	{
		free(game->map[i]);
		i++;
	}
	free(game->map);
	free_img(game);
	free_img2(game);
	free_img_player(game);
	if (game->mlx_ptr || game->win_ptr)
	{
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
	}
}
