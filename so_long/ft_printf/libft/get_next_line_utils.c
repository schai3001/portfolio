/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:06:05 by schai             #+#    #+#             */
/*   Updated: 2022/12/03 14:49:48 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	n;

	n = 0;
	while (str[n] != '\0')
		n++;
	return (n);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		size;
	char	*str;
	int		i;
	int		j;

	i = 0;
	size = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[size] = '\0';
	return (str);
}

char	*ft_strchr(const char *str, int n)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (char)n)
			return ((char *)(str + i));
		i++;
	}
	if (n == 0)
		return ((char *)(str + i));
	return (0);
}

void	*ft_calloc2(size_t nbr, size_t size)
{
	void	*t;

	if ((nbr != 0) && (size != 0) && (nbr * size) / size != nbr)
		return (NULL);
	t = (void *)malloc(nbr * size);
	if (!t || nbr >= SIZE_MAX || size >= SIZE_MAX)
	{
		free(t);
		return (NULL);
	}
	ft_bzero2(t, (nbr * size));
	return (t);
}

void	ft_bzero2(void *s, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = '\0';
		i++;
	}
}
