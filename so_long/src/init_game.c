/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 18:54:19 by schai             #+#    #+#             */
/*   Updated: 2022/12/28 19:10:47 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "so_long.h"
#include <stdlib.h>

void	refresh_pos(t_game *game, t_ull current_x, t_ull current_y)
{
	put_img_tile(game, current_x, current_y);
}

void	move_to_new_pos(t_game *game, t_ull new_dir, t_ull new_x, t_ull new_y)
{
	game->player.dir = new_dir;
	if (game->map[new_x][new_y] != '1')
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_player[game->player.dir][game->movement % 2].img_ptr,
		64 * (game->player.y), 64 * (game->player.x));
}

void	check_new_pos(t_game *game)
{
	if (game->map[game->player.x][game->player.y] == 'E')
	{
		if (game->count_collectible == game->collectible)
		{
			display_victory(game);
		}
	}
	else if (game->map[game->player.x][game->player.y] == 'K')
	{
		display_defeat(game);
	}
	else if (game->map[game->player.x][game->player.y] == 'C')
	{
		(game->count_collectible)++;
		game->map[game->player.x][game->player.y] = '0';
		put_img_tile(game, game->player.x, game->player.y);
		mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->img_player[game->player.dir][game->movement % 2].img_ptr,
			64 * (game->player.y), 64 * (game->player.x));
		if (game->count_collectible == game->collectible)
		{
			put_exit(game, 1);
		}
	}
}

int	press_close(t_game *game)
{
	free_game(game);
	exit(EXIT_SUCCESS);
	return (0);
}

void	init_game(t_game *game)
{
	game->count_collectible = 0;
	check_init(game);
	put_tiles(game);
	put_walls(game);
	load_player_img(game);
	game->player.x = find_player_x(game);
	game->player.y = find_player_y(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_player[0][0].img_ptr, 64 * (game->player.y),
		64 * (game->player.x));
	mlx_set_font(game->mlx_ptr, game->win_ptr,
		"-misc-fixed-bold-r-normal--15-140-75-75-c-90-iso8859-1");
	mlx_string_put(game->mlx_ptr, game->win_ptr,
		5, 20, 0xfffffff, "Move : 0");
	mlx_hook(game->win_ptr, EVENT_PRESS_KEY, 1L << 0, press_key, game);
	mlx_hook(game->win_ptr, EVENT_CLOSE, 0, press_close, game);
	mlx_loop_hook(game->mlx_ptr, animation, game);
	mlx_loop(game->mlx_ptr);
}
