/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:59:13 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:16:35 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*get_texture_path(char *line, int j)
{
	int		len;
	int		i;
	char	*path;

	while (line[j] && (line[j] == ' ' || line[j] == '\t'))
		j++;
	len = j;
	while (line[len] && (line[len] != ' ' && line[len] != '\t'))
		len++;
	path = malloc(sizeof(char) * (len - j + 1));
	if (!path)
		return (NULL);
	i = 0;
	while (line[j] && (line[j] != ' ' && line[j] != '\t' && line[j] != '\n'))
		path[i++] = line[j++];
	path[i] = '\0';
	while (line[j] && (line[j] == ' ' || line[j] == '\t'))
		j++;
	if (line[j] && line[j] != '\n')
	{
		free(path);
		path = NULL;
	}
	return (path);
}

static int	get_wall_textures(t_texturedata *textures, char *line, int j)
{
	if (line[j + 2] && ft_isprint(line[j + 2]))
		return (ERR);
	if (line[j] == 'N' && line[j + 1] == 'O' && !(textures->north))
		textures->north = get_texture_path(line, j + 2);
	else if (line[j] == 'S' && line[j + 1] == 'O' && !(textures->south))
		textures->south = get_texture_path(line, j + 2);
	else if (line[j] == 'W' && line[j + 1] == 'E' && !(textures->west))
		textures->west = get_texture_path(line, j + 2);
	else if (line[j] == 'E' && line[j + 1] == 'A' && !(textures->east))
		textures->east = get_texture_path(line, j + 2);
	else
		return (ERR);
	return (SUCCESS);
}

static int	mapdata_separation(t_data *data, char **map, int i, int j)
{
	while (map[i][j] == ' ' || map[i][j] == '\t' || map[i][j] == '\n')
		j++;
	if (ft_isprint(map[i][j]) && !ft_isdigit(map[i][j]))
	{
		if (map[i][j + 1] && ft_isprint(map[i][j + 1])
			&& !ft_isdigit(map[i][j]))
		{
			if (get_wall_textures(&data->texturedata, map[i], j) == ERR)
				return (error_msg(NULL, INV_TEXTURE, FAILURE));
			return (BREAK);
		}
		else
		{
			if (get_rgb_texture(&data->texturedata, map[i], j) == ERR)
				return (FAILURE);
			return (BREAK);
		}
	}
	else if (ft_isdigit(map[i][j]))
	{
		if (create_map(data, map, i) == FAILURE)
			return (error_msg(NULL, INV_MAP, FAILURE));
		return (SUCCESS);
	}
	return (CONTINUE);
}

int	get_file_data(t_data *data, char **map)
{
	int	i;
	int	j;
	int	state;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			state = mapdata_separation(data, map, i, j);
			if (state == BREAK)
				break ;
			else if (state == FAILURE)
				return (FAILURE);
			else if (state == SUCCESS)
				return (SUCCESS);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
