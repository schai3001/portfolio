/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:47:46 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:38:40 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_tab(void **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
}

static void	free_map(t_data *data)
{
	if (data->mapdata.fd > 0)
		close(data->mapdata.fd);
	if (data->mapdata.file)
		free_tab((void **)data->mapdata.file);
	if (data->map)
		free_tab((void **)data->map);
}

static void	free_texturedata(t_texturedata *textures)
{
	if (textures->north)
		free(textures->north);
	if (textures->south)
		free(textures->south);
	if (textures->west)
		free(textures->west);
	if (textures->east)
		free(textures->east);
	if (textures->floor)
		free(textures->floor);
	if (textures->ceiling)
		free(textures->ceiling);
}

int	free_data(t_data *data)
{
	if (data->texture_tab)
		free_tab((void **)data->texture_tab);
	if (data->texture_frame)
		free_tab((void **)data->texture_frame);
	free_texturedata(&data->texturedata);
	free_map(data);
	return (FAILURE);
}
