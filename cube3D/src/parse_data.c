/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 15:20:47 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:38:29 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_nb_of_lines(char *path)
{
	int		fd;
	char	*line;
	int		line_count;

	line_count = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_msg(path, strerror(errno), errno);
	else
	{
		line = get_next_line(fd);
		while (line != NULL)
		{
			line_count++;
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	return (line_count);
}

static void	fill_file(int row, int column, int i, t_data *data)
{
	char	*line;

	line = get_next_line(data->mapdata.fd);
	while (line != NULL)
	{
		data->mapdata.file[row] = ft_calloc(ft_strlen(line) + 1, sizeof(char));
		if (!data->mapdata.file[row])
		{
			error_msg(NULL, ERR_MALLOC, 0);
			return (free_tab((void **)data->mapdata.file));
		}
		while (line[i] != '\0')
			data->mapdata.file[row][column++] = line[i++];
		data->mapdata.file[row++][column] = '\0';
		column = 0;
		i = 0;
		free(line);
		line = get_next_line(data->mapdata.fd);
	}
	data->mapdata.file[row] = NULL;
}

void	parse_mapdata(char *path, t_data *data)
{
	int		row;
	int		i;
	size_t	column;

	i = 0;
	row = 0;
	column = 0;
	data->mapdata.nb_line = get_nb_of_lines(path);
	data->mapdata.path = path;
	data->mapdata.file = ft_calloc(data->mapdata.nb_line + 1, sizeof(char *));
	if (!(data->mapdata.file))
	{
		error_msg(NULL, ERR_MALLOC, 0);
		return ;
	}
	data->mapdata.fd = open(path, O_RDONLY);
	if (data->mapdata.fd < 0)
		error_msg(path, strerror(errno), FAILURE);
	else
	{
		fill_file(row, column, i, data);
		close(data->mapdata.fd);
	}
}
