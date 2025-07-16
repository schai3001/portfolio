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

static int	finish_key(int key_in, t_game *game)
{
	if (key_in == KEY_ESC)
	{
		free_game(game);
		exit(EXIT_SUCCESS);
		return (0);
	}
	else
		return (0);
}

void	display_victory(t_game *game)
{
	delete_enemy(game);
	delete_collectible(game);
	put_img_victory(game);
	mlx_hook(game->win_ptr, EVENT_PRESS_KEY, 1L << 0, finish_key, game);
}

void	put_img_victory(t_game *game)
{
	if (game->img_victory.img_ptr == NULL)
	{
		game->img_victory.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/victory.xpm", &(game->img_victory.width),
				&(game->img_victory.heigth));
		if (game->img_victory.img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting victory image !\nExit Program ");
		}
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_victory.img_ptr, ((game->width * 64) / 2) - 1280,
		((game->heigth * 64) / 2) - 750);
}
