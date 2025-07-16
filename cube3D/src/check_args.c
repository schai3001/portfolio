/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 13:17:49 by schai             #+#    #+#             */
/*   Updated: 2024/01/22 12:34:50 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	check_is_dir(char *arg)
{
	int		fd;
	bool	state;

	state = false;
	fd = open(arg, O_DIRECTORY);
	if (fd >= 0)
	{
		close (fd);
		state = true;
	}
	return (state);
}

static bool	check_extension(char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[len - 3] != 'c' || arg[len - 2] != 'u'
			|| arg[len - 1] != 'b'
			|| arg[len - 4] != '.'))
		return (false);
	return (true);
}

static bool	check_is_xpm(char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[len - 3] != 'x' || arg[len - 2] != 'p'
			|| arg[len - 1] != 'm'
			|| arg[len - 4] != '.'))
		return (false);
	return (true);
}

int	check_file(char *arg, bool state)
{
	int	fd;

	if (check_is_dir(arg))
		return (error_msg(arg, FILE_DIR, FAILURE));
	fd = open(arg, O_RDONLY);
	if (fd == -1)
		return (error_msg(arg, strerror(errno), FAILURE));
	close (fd);
	if (state && !check_extension(arg))
		return (error_msg(NULL, INV_EXTENSION, FAILURE));
	if (!state && !check_is_xpm(arg))
		return (error_msg(NULL, INV_XPM, FAILURE));
	return (SUCCESS);
}
