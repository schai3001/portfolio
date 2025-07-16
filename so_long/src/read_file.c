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

void	check_map_name(int argc, char **argv)
{
	char			*ber;
	unsigned int	len;

	if (argc != 2)
		exit_with_error("Error in Argument !\nExit Program ");
	len = ft_strlen(argv[1]);
	if (len < 4)
		exit_with_error("Error in File Name !\nExit Program ");
	ber = ft_substr(argv[1], len - 4, 4);
	if (ft_strncmp(ber, ".ber", 4) != 0)
	{
		free(ber);
		exit_with_error("Error in File extension !\nExit Program ");
	}
	else
		free(ber);
}

int	get_map_string_size(char *filename)
{
	int		fd;
	int		size;
	int		i;
	char	buffer[4096];

	size = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error in opening file !\nExit Program ");
	while (1)
	{
		i = read(fd, buffer, 4096);
		if (i < 0)
			exit_with_error("Error in reading file !\nExit Program ");
		if (i == 0)
			break ;
		size += i;
	}
	if (size == 0)
		exit_with_error("Error empty file !\nExit Program ");
	close(fd);
	return (size);
}

char	*read_map_string(char *filename)
{
	int		fd;
	int		i;
	int		size;
	char	*map_string;

	size = get_map_string_size(filename);
	map_string = (char *)malloc(sizeof(char) * (size + 1));
	if (!map_string)
		exit_with_error("Error in malloc buffer !\nExit Program ");
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		free(map_string);
		exit_with_error("Error in opening file !\nExit Program ");
	}
	i = read(fd, map_string, size + 1);
	if (i < 0)
	{
		free(map_string);
		exit_with_error("Error in reading file !\nExit Program ");
	}
	close(fd);
	map_string[size] = '\0';
	return (map_string);
}

int	check_newline(char *s)
{
	t_ull	i;
	t_ull	j;

	i = 0;
	j = ft_strlen(s) - 1;
	while (s[i] && s[i] == '\n')
		i++;
	while (s[j] && s[j] == '\n')
		j--;
	while (s[i] && i < j)
	{
		if (s[i] == '\n' && s[i + 1] == '\n')
			return (1);
		i++;
	}
	return (0);
}
