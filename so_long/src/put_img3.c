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

void	put_img_wall(t_game *game, t_ull i, t_ull j, t_ull k)
{
	if (game->img_wall[0][0].img_ptr == NULL ||
		game->img_wall[2][0].img_ptr == NULL ||
		game->img_wall[3][0].img_ptr == NULL ||
		game->img_wall[4][0].img_ptr == NULL ||
		game->img_wall[5][0].img_ptr == NULL ||
		game->img_wall[6][0].img_ptr == NULL ||
		game->img_wall[1][0].img_ptr == NULL ||
		game->img_wall[7][0].img_ptr == NULL)
	{
		load_wall_img(game);
		if (game->img_wall[0][0].img_ptr == NULL ||
			game->img_wall[2][0].img_ptr == NULL ||
			game->img_wall[3][0].img_ptr == NULL ||
			game->img_wall[4][0].img_ptr == NULL ||
			game->img_wall[5][0].img_ptr == NULL ||
			game->img_wall[6][0].img_ptr == NULL ||
			game->img_wall[1][0].img_ptr == NULL ||
			game->img_wall[7][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting walls image !\nExit Program ");
		}
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_wall[k][0].img_ptr, 64 * j, 64 * i);
}

void	put_img_enemy(t_game *game, t_ull i, t_ull j)
{
	if (game->img_enemy[0][0].img_ptr == NULL)
	{
		game->img_enemy[0][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/enemy/enemy0.xpm", &(game->img_enemy[0][0].width),
				&(game->img_enemy[0][0].heigth));
		if (game->img_enemy[0][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting enemy image !\nExit Program ");
		}
	}
	if (game->img_enemy[1][0].img_ptr == NULL)
	{
		game->img_enemy[1][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/enemy/enemy1.xpm", &(game->img_enemy[1][0].width),
				&(game->img_enemy[1][0].heigth));
		if (game->img_enemy[1][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting enemy image !\nExit Program ");
		}
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_enemy[game->enemy_frames][0].img_ptr, 64 * j, 64 * i);
}
