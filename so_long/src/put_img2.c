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

static void	put_img_collect4(t_game *game)
{
	if (game->img_collect[5][0].img_ptr == NULL)
	{
		game->img_collect[5][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible5.xpm",
				&(game->img_collect[5][0].width),
				&(game->img_collect[5][0].heigth));
		if (game->img_collect[5][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
	if (game->img_collect[6][0].img_ptr == NULL)
	{
		game->img_collect[6][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible6.xpm",
				&(game->img_collect[6][0].width),
				&(game->img_collect[6][0].heigth));
		if (game->img_collect[6][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
}

static void	put_img_collect3(t_game *game)
{
	if (game->img_collect[3][0].img_ptr == NULL)
	{
		game->img_collect[3][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible3.xpm",
				&(game->img_collect[3][0].width),
				&(game->img_collect[3][0].heigth));
		if (game->img_collect[3][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
	if (game->img_collect[4][0].img_ptr == NULL)
	{
		game->img_collect[4][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible4.xpm",
				&(game->img_collect[4][0].width),
				&(game->img_collect[4][0].heigth));
		if (game->img_collect[4][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
}

static void	put_img_collect2(t_game *game)
{
	if (game->img_collect[1][0].img_ptr == NULL)
	{
		game->img_collect[1][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible1.xpm",
				&(game->img_collect[1][0].width),
				&(game->img_collect[1][0].heigth));
		if (game->img_collect[1][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
	if (game->img_collect[2][0].img_ptr == NULL)
	{
		game->img_collect[2][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible2.xpm",
				&(game->img_collect[2][0].width),
				&(game->img_collect[2][0].heigth));
		if (game->img_collect[2][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
}

void	put_img_collect(t_game *game, t_ull i, t_ull j)
{
	if (game->img_collect[0][0].img_ptr == NULL)
	{
		game->img_collect[0][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/collectible/collectible0.xpm",
				&(game->img_collect[0][0].width),
				&(game->img_collect[0][0].heigth));
		if (game->img_collect[0][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with collectible image !\nExit Program ");
		}
	}
	put_img_collect2(game);
	put_img_collect3(game);
	put_img_collect4(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_collect[game->collect_frames / 2000][0].img_ptr,
		64 * j, 64 * i);
}
