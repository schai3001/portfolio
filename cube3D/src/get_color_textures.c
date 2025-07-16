/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color_textures.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 17:33:41 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:23:39 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	check_digit(char *str)
{
	int		i;
	bool	state;

	i = 0;
	state = true;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 1)
			state = false;
		i++;
	}
	return (state);
}

static int	*get_rgb(char **rgb_tab, int *rgb)
{
	int		i;

	i = -1;
	while (rgb_tab[++i])
	{
		rgb[i] = ft_atoi(rgb_tab[i]);
		if (rgb[i] == -1 || check_digit(rgb_tab[i]) == true)
		{
			free_tab((void **)rgb_tab);
			free(rgb);
			return (0);
		}
	}
	free_tab((void **)rgb_tab);
	return (rgb);
}

static int	*set_rgb_colors(char *line)
{
	char	**rgb_tab;
	int		*rgb;
	int		i;

	rgb_tab = ft_split(line, ',');
	i = 0;
	while (rgb_tab[i])
		i++;
	if (i != 3)
	{
		free_tab((void **)rgb_tab);
		return (0);
	}
	rgb = malloc(sizeof(int) * 3);
	if (!rgb)
	{
		error_msg(NULL, ERR_MALLOC, 0);
		return (0);
	}
	return (get_rgb(rgb_tab, rgb));
}

int	get_rgb_texture(t_texturedata *textures, char *line, int j)
{
	if (line[j + 1] && ft_isprint(line[j + 1]))
		return (error_msg(NULL, INV_FLOOR_CEILING, ERR));
	if (!textures->ceiling && line[j] == 'C')
	{
		textures->ceiling = set_rgb_colors(line + j + 1);
		if (textures->ceiling == 0)
			return (error_msg(NULL, INV_CEILING, ERR));
	}
	else if (!textures->floor && line[j] == 'F')
	{
		textures->floor = set_rgb_colors(line + j + 1);
		if (textures->floor == 0)
			return (error_msg(NULL, INV_FLOOR, ERR));
	}
	else
		return (error_msg(NULL, INV_FLOOR_CEILING, ERR));
	return (SUCCESS);
}
