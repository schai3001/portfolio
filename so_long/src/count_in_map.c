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

int	count_player(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		n;

	i = 0;
	n = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'P')
				n++;
			j++;
		}
		i++;
	}
	return (n);
}

int	count_exit(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		n;

	i = 0;
	n = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'E')
				n++;
			j++;
		}
		i++;
	}
	return (n);
}

int	count_collect(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		n;

	i = 0;
	n = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'C')
			n++;
		j++;
		}
	i++;
	}
	return (n);
}

int	count_enemy(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		n;

	i = 0;
	n = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == 'K')
				n++;
			j++;
		}
		i++;
	}
	return (n);
}

int	count_parasite(t_game *game)
{
	t_ull	i;
	t_ull	j;
	int		n;

	i = 0;
	n = 0;
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] != 'K' && game->map[i][j] != '1'
					&& game->map[i][j] != '0'
					&& game->map[i][j] != 'P' && game->map[i][j] != 'C'
					&& game->map[i][j] != 'E')
				n++;
			j++;
		}
		i++;
	}
	return (n);
}
