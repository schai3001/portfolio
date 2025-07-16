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

#ifndef SO_LONG_H
# define SO_LONG_H

# include <errno.h>
# include <mlx.h>
# include <mlx_int.h>
# include "ft_printf.h"
# include "libft.h"
# include <errno.h>

# define EVENT_PRESS_KEY 2
# define EVENT_CLOSE 17
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100

typedef unsigned long long	t_ull;

typedef struct s_image
{
	void	*img_ptr;
	int		heigth;
	int		width;
}	t_image;

typedef struct s_pos
{
	t_ull	dir;
	t_ull	x;
	t_ull	y;
}	t_pos;

typedef struct s_game
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_ull	heigth;
	t_ull	width;
	char	**map;
	char	*move;
	char	*move2;
	t_ull	collectible;
	t_ull	exit;
	t_ull	start;
	t_ull	enemy;
	t_ull	movement;
	t_ull	count_mov;
	t_ull	count_collectible;
	t_image	img_tile;
	t_image	img_wall[8][1];
	t_image	img_collect[7][1];
	t_ull	collect_frames;
	t_ull	enemy_frames;
	t_image	img_exit;
	t_image	img_enemy[2][1];
	t_image	img_player[4][2];
	t_image	img_victory;
	t_image	img_defeat;
	t_pos	player;
	t_pos	test;
}		t_game;

void	init_game(t_game *game);
void	exit_with_error(char *msg);
void	check_map(t_game *game);
void	check_map_name(int argc, char **argv);
void	check_path_collect(t_game *game);
void	check_path_exit(t_game *game);
void	put_img_tile(t_game *game, t_ull i, t_ull j);
void	put_img_exit(t_game *game, t_ull i, t_ull j, int flag);
void	put_img_wall(t_game *game, t_ull i, t_ull j, t_ull k);
void	put_img_collect(t_game *game, t_ull i, t_ull j);
void	put_tiles(t_game *game);
void	put_exit(t_game *game, int flag);
void	put_walls(t_game *game);
void	put_collectible(t_game *game);
void	load_player_img(t_game *game);
void	load_wall_img(t_game *game);
void	put_img_victory(t_game *game);
void	put_img_defeat(t_game *game);
void	free_game(t_game *game);
void	display_victory(t_game *game);
void	display_defeat(t_game *game);
void	load_img_collect(t_game *game, t_ull i, t_ull j, t_ull k);
void	put_enemy(t_game *game);
void	put_img_enemy(t_game *game, t_ull i, t_ull j);
void	delete_enemy(t_game *game);
void	delete_collectible(t_game *game);
void	refresh_pos(t_game *game, t_ull current_x, t_ull current_y);
void	check_new_pos(t_game *game);
void	move_to_new_pos(t_game *game, t_ull new_dir, t_ull new_x, t_ull new_y);
void	check_game_size(t_game *game);
void	check_walls_v2(t_game *game);
void	check_walls_v(t_game *game);
void	check_walls_h2(t_game *game);
void	check_walls_h(t_game *game);
void	check_map2(t_game *game);
void	create_map_int2(int **map_int, t_game *game);
void	create_map_visited2(int **map_visited, int **map_int, t_game *game);
void	reset_visited(t_game *game, int **map_visited, int **map_int);
void	check_path_collect2(t_game *game, int **map_int, int **map_visited);
void	create_map_int2_e(int **map_int, t_game *game);
void	check_init(t_game *game);

char	*read_map_string(char *filename);

int		get_map_string_size(char *filename);
int		count_player(t_game *game);
int		count_exit(t_game *game);
int		count_collect(t_game *game);
int		count_enemy(t_game *game);
int		count_parasite(t_game *game);
int		animation(t_game *game);
int		press_key(int key_in, t_game *game);
int		is_path_e(int **map_int, t_ull i, t_ull j, int **map_visited);
int		check_newline(char *s);

t_ull	find_player_x(t_game *game);
t_ull	find_player_y(t_game *game);

#endif