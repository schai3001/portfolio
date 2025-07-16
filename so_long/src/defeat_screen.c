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
	}
	return (0);
}

void	delete_enemy(t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'K')
			{
				game->map[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}

void	delete_collectible(t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'C')
			{
				game->map[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}

void	display_defeat(t_game *game)
{
	delete_enemy(game);
	delete_collectible(game);
	put_img_defeat(game);
	mlx_hook(game->win_ptr, EVENT_PRESS_KEY, 1L << 0, finish_key, game);
}

void	put_img_defeat(t_game *game)
{
	if (game->img_defeat.img_ptr == NULL)
	{
		game->img_defeat.img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
				"textures/defeat.xpm", &(game->img_defeat.width),
				&(game->img_defeat.heigth));
		if (game->img_defeat.img_ptr == NULL)
		{
			free_game(game);
			exit_with_error("Error with getting defeat image !\nExit Program ");
		}
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_defeat.img_ptr, ((game->width * 64) / 2) - 1280,
		((game->heigth * 64) / 2) - 720);
}
