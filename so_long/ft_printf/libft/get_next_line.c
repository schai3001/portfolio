/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 15:25:51 by schai             #+#    #+#             */
/*   Updated: 2022/11/16 17:36:19 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

/*cherche le prochain \n du stock et la coupe pour mettre 
le reste dans line qui devient le nouveau stock*/
char	*ft_refresh_stock(char *stock)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	while (stock[i] && stock[i] != '\n')
		i++;
	if (!stock[i])
	{
		free(stock);
		return (NULL);
	}
	line = ft_calloc2(ft_strlen(stock) - i + 1, sizeof(char));
	i++;
	while (stock[i])
	{
		line[j] = stock[i];
		i++;
		j++;
	}
	free(stock);
	return (line);
}

//cherche le \n ou la fin du stock, 
//creer line en fonction de la taille trouver
//copie le stock dans line jusqua \n ou la fin, 
//si il trouve un \n alors le copie aussi et renvoie line

char	*ft_get_newline(char *stock)
{
	char	*line;
	int		i;

	i = 0;
	if (!stock[i])
		return (NULL);
	while (stock[i] && stock[i] != '\n')
		i++;
	line = ft_calloc2(i + 2, sizeof(char));
	i = 0;
	while (stock[i] && stock[i] != '\n')
	{
		line[i] = stock[i];
		i++;
	}
	if (stock[i] && stock[i] == '\n')
		line[i++] = '\n';
	return (line);
}

//prend le stock existant et le buffer et les join, 
//free le stock existant et ressort le nouveau stock (ancien stock + buffer)

char	*ft_join_free(char *stock, char *buffer)
{
	char	*newstock;

	newstock = ft_strjoin(stock, buffer);
	free(stock);
	return (newstock);
}

//calloc stock si il nexiste pas, malloc buffer et 
//read pour ecrire dans le buffer ce qui a ete lu
//si on arrive a la fin, free le buffer creer et renvoie null
//plaec un '\0' a la fin de buffer
//associe a stock le nouveau stock (ancin stock + buffer creer)
//si il y avait un \n dans le buffer on stop pour couper le stock 
//et afficher tout depuis le debut jusquau \n
//free le buffer a la fin et return newstock

char	*ft_read_stock(int fd, char *st)
{
	char	*buff;
	int		byte_lu;

	if (!st)
		st = ft_calloc2(1, 1);
	buff = ft_calloc2(BUFFER_SIZE + 1, sizeof(char));
	if (!buff)
		return (NULL);
	byte_lu = 1;
	while (byte_lu != 0)
	{
		byte_lu = read(fd, buff, BUFFER_SIZE);
		if (byte_lu == -1)
		{
			free(buff);
			free(st);
			return (NULL);
		}
		buff[byte_lu] = '\0';
		st = ft_join_free(st, buff);
		if (ft_strchr(buff, '\n'))
			break ;
	}
	free(buff);
	return (st);
}

//verifie que le fichier est bien ouvert et peut etre lu, 
//creer le static char stock
//avec read_stock on obtient un stock jusqua \n ou la fin, 
//verifie les cas d'erreur

char	*get_next_line(int fd)
{
	static char	*stock;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	stock = ft_read_stock(fd, stock);
	if (!stock)
		return (NULL);
	line = ft_get_newline(stock);
	stock = ft_refresh_stock(stock);
	return (line);
}
/*
int main()
{
	int	fd;
	char	*line;

	fd = open ("text.txt", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
		{
			printf("%s", line);
			break;
		}
		printf("%s", line);
		free(line);
	}
	return (0);
}*/
