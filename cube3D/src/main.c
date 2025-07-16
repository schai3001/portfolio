/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:22 by schai             #+#    #+#             */
/*   Updated: 2024/02/05 11:35:30 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	error_msg(char *context, char *s, int code)
{
	ft_putstr_fd("ERROR", 2);
	if (context)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(context, 2);
	}
	if (s)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(s, 2);
	}
	return (code);
}

void	clean_exit(t_data *data, int status)
{
	if (!data)
		exit(status);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		mlx_loop_end(data->mlx);
		free(data->mlx);
	}
	free_data(data);
	exit(status);
}

int	exit_cub(t_data *data)
{
	clean_exit(data, 0);
	return (0);
}

static int	parse_args(t_data *data, char **av)
{
	if (check_file(av[1], true) == FAILURE)
		clean_exit(data, FAILURE);
	parse_mapdata(av[1], data);
	if (get_file_data(data, data->mapdata.file) == FAILURE)
		return (free_data(data));
	if (check_map(data, data->map) == FAILURE)
		return (free_data(data));
	if (check_textures(&data->texturedata) == FAILURE)
		return (free_data(data));
	init_player(data);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)
		return (error_msg(NULL, INV_USAGE, 1));
	init_data(&data);
	if (parse_args(&data, av) != 0)
		return (1);
	init_mlx(&data);
	init_textures(&data);
	start_game(&data);
	input(&data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);
	return (0);
}
