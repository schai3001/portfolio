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

void	put_img_tile(t_game *game, t_ull i, t_ull j)
{
	if (game->img_tile.img_ptr == NULL)
	{
		game->img_tile.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/tile.xpm", &(game->img_tile.width),
				&(game->img_tile.heigth));
		if (game->img_tile.img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting tiles image !\nExit Program ");
		}
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_tile.img_ptr, 64 * j, 64 * i);
}

void	load_img_exit(t_game *game, int flag)
{
	void	*tmp_ptr;

	if (flag == 0)
	{
		if (game->img_exit.img_ptr == NULL)
			game->img_exit.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
					"textures/exit1.xpm", &(game->img_exit.width),
					&(game->img_exit.heigth));
	}
	else
	{
		tmp_ptr = game->img_exit.img_ptr;
		game->img_exit.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/exit.xpm", &(game->img_exit.width),
				&(game->img_exit.heigth));
		if (tmp_ptr)
			mlx_destroy_image(game->mlx_ptr, tmp_ptr);
	}
}

void	put_img_exit(t_game *game, t_ull i, t_ull j, int flag)
{
	load_img_exit(game, flag);
	if (game->img_exit.img_ptr == NULL)
	{
		free_game(game);
		exit_with_error("Error with getting exit image !\nExit Program ");
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_exit.img_ptr, 64 * j, 64 * i);
}
