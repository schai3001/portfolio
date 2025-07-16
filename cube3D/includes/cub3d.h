/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:20 by schai             #+#    #+#             */
/*   Updated: 2024/02/05 16:18:14 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <X11/keysym.h>
# include <X11/X.h>

# define WIN_WIDTH 960
# define WIN_HEIGHT 720

# define MINIMAP_DISTANCE 4
# define MINIMAP_SQUARE 128

# define MINIMAP_BLACK 0x090909
# define MINIMAP_PLAYER 0xF30408
# define MINIMAP_WALL 0x1BF50C
# define MINIMAP_FLOOR 0x0F7908

# define TEXTURE_SIZE 64

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define MS 0.0400
# define RS 0.030
# define MOUSE 20

# define INV_USAGE "cub3d <pathtomap.cub>"
# define FILE_DIR "file is a directory"
# define INV_EXTENSION "Not a .cub file"
# define INV_XPM "Not an .xpm file"
# define ERR_MALLOC "Error in allocating memory"

# define INV_FLOOR_CEILING "Invalid floor or ceiling RGB colors"
# define INV_FLOOR "Invalid floor RGB color"
# define INV_CEILING "Invalid ceiling RGB color"

# define INV_MAP "Invalid Map"
# define MAP_MISSING "Missing map"
# define MAP_NO_WALLS "Map is not surrounded by walls"
# define MAP_TOO_SMALL "Map is not at least 3 lines high"
# define MAP_LAST_ELEM "Map is not the last element in file"
# define WRONG_CHAR "Wrong char in map"
# define INV_PLAYER_POS "Invalid player position"

# define MISS_TEXTURE "Missing Textures"
# define INV_TEXTURE "Invalid Textures"
# define INV_RGB "Invalid RGB value"
# define MISS_COLOR "Missing Color"

# define ERROR_MLX "Error mlx init"
# define ERROR_WIN "Error mlx window"

# define MLX_IMG "Error mlx img"

enum e_output
{
	SUCCESS = 0,
	FAILURE = 1,
	ERR = 2,
	BREAK = 3,
	CONTINUE = 4
};

typedef struct s_mapdata
{
	int		fd;
	int		nb_line;
	char	*path;
	char	**file;
	int		height;
	int		width;
	int		index_end_of_map;
}	t_mapdata;

typedef struct s_imgdata
{
	void	*img;
	int		*address;
	int		pixel_bits;
	int		size_line;
	int		endian;
}	t_imgdata;

typedef struct s_texturedata
{
	char			*north;
	char			*south;
	char			*west;
	char			*east;
	int				*floor;
	int				*ceiling;
	unsigned long	hex_floor;
	unsigned long	hex_ceiling;
	int				size;
	int				index;
	double			step;
	double			pos;
	int				x;
	int				y;
}	t_texturedata;

typedef struct s_playerdata
{
	char	dir;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	int		check_move;
	int		move_x;
	int		move_y;
	int		rot;
}	t_playerdata;

typedef struct s_raydata
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	double	sidedist_x;
	double	sidedist_y;
	double	deltadist_x;
	double	deltadist_y;
	double	wall_dist;
	double	wall_x;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_raydata;

typedef struct s_minimapdata
{
	char		**map_tab;
	t_imgdata	*img;
	int			size;
	int			x;
	int			y;
	int			distance;
	int			square_size;
}	t_minimapdata;

typedef struct s_data
{
	void			*mlx;
	void			*win;
	int				win_width;
	int				win_height;
	char			**map;
	t_mapdata		mapdata;
	t_texturedata	texturedata;
	t_playerdata	playerdata;
	int				**texture_tab;
	int				**texture_frame;
	t_raydata		raydata;
	t_imgdata		minimap;
}	t_data;

int		get_rgb_texture(t_texturedata *textures, char *line, int j);

void	init_data(t_data *data);

int		check_file(char *arg, bool state);

int		error_msg(char *context, char *s, int code);

void	parse_mapdata(char *path, t_data *data);

size_t	find_longest_line(t_mapdata *map, int i);

int		get_file_data(t_data *data, char **map);

int		create_map(t_data *data, char **file, int i);

int		check_is_whitespace(char c);

int		check_map(t_data *data, char **map_tab);

int		check_textures(t_texturedata *textures);

int		free_data(t_data *data);

void	free_tab(void **tab);

void	init_mlx(t_data *data);

void	clean_exit(t_data *data, int status);

void	init_textures(t_data *data);

int		exit_cub(t_data *data);

void	input(t_data *data);

void	init_texture_img(t_data *data, t_imgdata *imgdata, char *path);

void	render_images(t_data *data);

void	update_frame(t_data *data, t_texturedata *tex, t_raydata *ray, int x);

void	init_new_img(t_data *data, t_imgdata *image, int width, int height);

int		render(t_data *data);

void	init_player(t_data *data);

int		move_player(t_data *data);

void	render_raycast(t_data *data);

void	render_frame(t_data *data);

void	start_game(t_data *data);

void	init_dda(t_raydata *ray, t_playerdata *player);

void	start_dda(t_data *data, t_raydata *ray);

void	start_frame(t_data *data);

void	set_frame(t_data *data, t_imgdata *image, int x, int y);

int		raycasting(t_playerdata *playerdata, t_data *data);

void	set_pixel(t_imgdata *image, int x, int y, int color);

void	init_raydata(t_raydata *raydata);

int		rotate_player(t_data *data, double rotdir);

int		check_player_position(t_data *data, double new_x, double new_y);

int		check_map_walls(t_mapdata *mapdata, char **map_tab);

void	minimap_frame(t_data *data, t_minimapdata *minimap);

void	init_imgdata(t_imgdata *imgdata);

void	set_image_pixel(t_imgdata *image, int x, int y, int color);

void	start_minimap(t_data *data);

int		check_char(char c, char *valid_char);

int		check_vertical(t_data *data, char **map_tab);

int		check_horizontal(char **map_tab);

int		check_wall2(char **map_tab);

int		check_last_char(char **map_tab);

int		vertical_check(t_data *data, char **map_tab);

#endif