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

static void	put_text(t_game *game)
{
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_wall[0][0].img_ptr, 0, 0);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->img_wall[1][0].img_ptr, 64, 0);
	mlx_set_font(game->mlx_ptr, game->win_ptr,
		"-misc-fixed-bold-r-normal--15-140-75-75-c-90-iso8859-1");
	mlx_string_put(game->mlx_ptr, game->win_ptr,
		5, 20, 0xfffffff, game->move2);
}

static void	movement_text(t_game *game)
{
	if (game->movement == 1000)
	{
		free_game(game);
		exit_with_error("Too Much Movement !\nExit Program ");
	}
	ft_printf("number of movement : %d\n", game->movement);
	game->move = ft_itoa((int)game->movement);
	if (game->move == NULL)
	{
		free_game(game);
		exit_with_error("Error with Memory for itoa !\nExit Program ");
	}
	game->move2 = ft_strjoin("Move : ", game->move);
	free(game->move);
	if (game->move2 == NULL)
	{
		free_game(game);
		exit_with_error("Error with Memory for strjoin !\nExit Program ");
	}
	put_text(game);
	free(game->move2);
}

int	press_key(int key_in, t_game *game)
{
	if (key_in == KEY_ESC)
	{
		free_game(game);
		exit(EXIT_SUCCESS);
	}
	if (key_in == KEY_A || key_in == KEY_D
		|| key_in == KEY_S || key_in == KEY_W)
	{
		(game->movement)++;
		movement_text(game);
		refresh_pos(game, game->player.x, game->player.y);
		if (key_in == KEY_W)
			move_to_new_pos(game, 0, game->player.x - 1, game->player.y);
		else if (key_in == KEY_A)
			move_to_new_pos(game, 2, game->player.x, game->player.y - 1);
		else if (key_in == KEY_S)
			move_to_new_pos(game, 1, game->player.x + 1, game->player.y);
		else if (key_in == KEY_D)
			move_to_new_pos(game, 3, game->player.x, game->player.y + 1);
		check_new_pos(game);
	}
	return (0);
}

void	check_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	if (game->mlx_ptr == 0)
	{
		free_game(game);
		exit_with_error("Error with mlx init !\nExit Program ");
	}
	game->win_ptr = mlx_new_window(game->mlx_ptr, (game->width) * 64,
			(game->heigth) * 64, "FTL : Escape Mission");
	if (game->win_ptr == 0)
	{
		free_game(game);
		exit_with_error("Error with opening window !\nExit Program ");
	}
}
