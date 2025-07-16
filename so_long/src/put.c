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

void	put_walls(t_game *game)
{
	t_ull	i;
	t_ull	j;
	t_ull	k;

	k = 0;
	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == '1')
			{
				put_img_wall(game, i, j, k);
				k++;
				if (k == 8)
				{
					k = 0;
				}
			}
			j++;
		}
		i++;
	}
}

void	put_collectible(t_game *game)
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
				put_img_collect(game, i, j);
			j++;
		}
		i++;
	}
}

void	put_enemy(t_game *game)
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
				put_img_enemy(game, i, j);
			j++;
		}
		i++;
	}
}

void	put_tiles(t_game *game)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == '0')
			{
				put_img_tile(game, i, j);
			}
			j++;
		}
		i++;
	}
}

void	put_exit(t_game *game, int flag)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'E')
				put_img_exit(game, i, j, flag);
			j++;
		}
		i++;
	}
}
