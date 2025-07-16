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
#include <stdlib.h>
#include <stdio.h> 

void	exit_with_error(char *msg)
{
	ft_printf("Error\n");
	perror(msg);
	exit(EXIT_FAILURE);
}

static void	init3(t_game *game)
{
	game->mlx_ptr = NULL;
	game->win_ptr = NULL;
	game->heigth = 0;
	game->width = 0;
	game->map = NULL;
	game->collectible = 0;
	game->collect_frames = 0;
	game->exit = 0;
	game->start = 0;
	game->enemy = 0;
	game->enemy_frames = 0;
	game->movement = 0;
	game->count_collectible = 0;
	game->img_tile.img_ptr = NULL;
	game->player.x = 0;
	game->player.y = 0;
	game->player.dir = 0;
	game->move = NULL;
	game->img_exit.img_ptr = NULL;
	game->img_victory.img_ptr = NULL;
	game->img_defeat.img_ptr = NULL;
}

static void	init2(t_game *game)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		game->img_collect[i][0].img_ptr = NULL;
		i++;
	}
	i = 0;
	while (i < 2)
	{
		game->img_enemy[i][0].img_ptr = NULL;
		i++;
	}
}

static void	init(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < 8)
	{
		game->img_wall[i][0].img_ptr = NULL;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 2)
		{
			game->img_player[i][j].img_ptr = NULL;
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_game	game;
	char	*map_string;

	check_map_name(argc, argv);
	init(&game);
	init2(&game);
	init3(&game);
	map_string = read_map_string(argv[1]);
	if (check_newline(map_string) == 1)
	{
		free(map_string);
		exit_with_error("Error in map form !\nExit Program ");
	}
	game.map = ft_split(map_string, '\n');
	free(map_string);
	if (game.map == 0)
		exit_with_error("Error in splitting map !\nExit Program ");
	check_map(&game);
	init_game(&game);
	return (0);
}
