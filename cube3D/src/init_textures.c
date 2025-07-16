/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:57:22 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:27:35 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_imgdata(t_imgdata *imgdata)
{
	imgdata->img = NULL;
	imgdata->address = NULL;
	imgdata->pixel_bits = 0;
	imgdata->size_line = 0;
	imgdata->endian = 0;
}

void	init_new_img(t_data *data, t_imgdata *image, int width, int height)
{
	init_imgdata(image);
	image->img = mlx_new_image(data->mlx, width, height);
	if (image->img == NULL)
		clean_exit(data, error_msg(NULL, MLX_IMG, 1));
	image->address = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

void	init_texture_img(t_data *data, t_imgdata *imgdata, char *path)
{
	init_imgdata(imgdata);
	imgdata->img = mlx_xpm_file_to_image(data->mlx, path,
			&data->texturedata.size, &data->texturedata.size);
	if (imgdata->img == NULL)
		clean_exit(data, error_msg(NULL, MLX_IMG, 1));
	imgdata->address = (int *)mlx_get_data_addr(imgdata->img,
			&imgdata->pixel_bits, &imgdata->size_line, &imgdata->endian);
	return ;
}

static int	*convert_xpm(t_data *data, char *path)
{
	t_imgdata	tmp;
	int			*buf;
	int			x;
	int			y;

	init_texture_img(data, &tmp, path);
	buf = ft_calloc(1,
			sizeof * buf * data->texturedata.size * data->texturedata.size);
	if (!buf)
		clean_exit(data, error_msg(NULL, ERR_MALLOC, 1));
	y = 0;
	while (y < data->texturedata.size)
	{
		x = 0;
		while (x < data->texturedata.size)
		{
			buf[y * data->texturedata.size + x]
				= tmp.address[y * data->texturedata.size + x];
			++x;
		}
		y++;
	}
	mlx_destroy_image(data->mlx, tmp.img);
	return (buf);
}

void	init_textures(t_data *data)
{
	data->texture_tab = ft_calloc(5, sizeof * data->texture_tab);
	if (!data->texture_tab)
		clean_exit(data, error_msg(NULL, ERR_MALLOC, 1));
	data->texture_tab[0] = convert_xpm(data, data->texturedata.north);
	data->texture_tab[1] = convert_xpm(data, data->texturedata.south);
	data->texture_tab[2] = convert_xpm(data, data->texturedata.east);
	data->texture_tab[3] = convert_xpm(data, data->texturedata.west);
}
