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

static void	load_wall_img4(t_game *game)
{
	if (game->img_wall[6][0].img_ptr == NULL)
	{
		game->img_wall[6][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall6.xpm", &(game->img_wall[6][0].width),
				&(game->img_wall[6][0].heigth));
		if (game->img_wall[6][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
	if (game->img_wall[7][0].img_ptr == NULL)
	{
		game->img_wall[7][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall7.xpm", &(game->img_wall[7][0].width),
				&(game->img_wall[7][0].heigth));
		if (game->img_wall[7][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}	
}

static void	load_wall_img3(t_game *game)
{
	if (game->img_wall[4][0].img_ptr == NULL)
	{
		game->img_wall[4][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall4.xpm", &(game->img_wall[4][0].width),
				&(game->img_wall[4][0].heigth));
		if (game->img_wall[4][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
	if (game->img_wall[5][0].img_ptr == NULL)
	{
		game->img_wall[5][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall5.xpm", &(game->img_wall[5][0].width),
				&(game->img_wall[5][0].heigth));
		if (game->img_wall[5][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
}

static void	load_wall_img2(t_game *game)
{
	if (game->img_wall[2][0].img_ptr == NULL)
	{
		game->img_wall[2][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall2.xpm", &(game->img_wall[2][0].width),
				&(game->img_wall[2][0].heigth));
		if (game->img_wall[2][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
	if (game->img_wall[3][0].img_ptr == NULL)
	{
		game->img_wall[3][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall3.xpm", &(game->img_wall[3][0].width),
				&(game->img_wall[3][0].heigth));
		if (game->img_wall[3][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
}

void	load_wall_img(t_game *game)
{
	if (game->img_wall[0][0].img_ptr == NULL)
	{
		game->img_wall[0][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall0.xpm", &(game->img_wall[0][0].width),
				&(game->img_wall[0][0].heigth));
		if (game->img_wall[0][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
	if (game->img_wall[1][0].img_ptr == NULL)
	{
		game->img_wall[1][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/wall/wall1.xpm", &(game->img_wall[1][0].width),
				&(game->img_wall[1][0].heigth));
		if (game->img_wall[1][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting wall image !\nExit Program ");
		}
	}
	load_wall_img2(game);
	load_wall_img3(game);
	load_wall_img4(game);
}
