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

static void	load_player_up(t_game *game)
{
	if (game->img_player[0][0].img_ptr == NULL)
	{
		game->img_player[0][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_up0.xpm",
				&(game->img_player[0][0].width),
				&(game->img_player[0][0].heigth));
		if (game->img_player[0][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
	if (game->img_player[0][1].img_ptr == NULL)
	{
		game->img_player[0][1].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_up1.xpm",
				&(game->img_player[0][1].width),
				&(game->img_player[0][1].heigth));
		if (game->img_player[0][1].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
}

static void	load_player_down(t_game *game)
{
	if (game->img_player[1][0].img_ptr == NULL)
	{
		game->img_player[1][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_down0.xpm",
				&(game->img_player[1][0].width),
				&(game->img_player[1][0].heigth));
		if (game->img_player[1][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
	if (game->img_player[1][1].img_ptr == NULL)
	{
		game->img_player[1][1].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_down1.xpm",
				&(game->img_player[1][1].width),
				&(game->img_player[1][1].heigth));
		if (game->img_player[1][1].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
}

static void	load_player_left(t_game *game)
{
	if (game->img_player[2][0].img_ptr == NULL)
	{
		game->img_player[2][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_left0.xpm",
				&(game->img_player[2][0].width),
				&(game->img_player[2][0].heigth));
		if (game->img_player[2][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
	if (game->img_player[2][1].img_ptr == NULL)
	{
		game->img_player[2][1].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_left1.xpm",
				&(game->img_player[2][1].width),
				&(game->img_player[2][1].heigth));
		if (game->img_player[2][1].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
}

static void	load_player_right(t_game *game)
{
	if (game->img_player[3][0].img_ptr == NULL)
	{
		game->img_player[3][0].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_right0.xpm",
				&(game->img_player[3][0].width),
				&(game->img_player[3][0].heigth));
		if (game->img_player[3][0].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
	if (game->img_player[3][1].img_ptr == NULL)
	{
		game->img_player[3][1].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/player/player_right1.xpm",
				&(game->img_player[3][1].width),
				&(game->img_player[3][1].heigth));
		if (game->img_player[3][1].img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting player image !\nExit Program ");
		}
	}
}

void	load_player_img(t_game *game)
{
	load_player_up(game);
	load_player_down(game);
	load_player_left(game);
	load_player_right(game);
}
