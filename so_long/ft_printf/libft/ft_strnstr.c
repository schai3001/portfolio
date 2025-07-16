/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:41:56 by schai             #+#    #+#             */
/*   Updated: 2022/11/12 11:23:57 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

static int	is_in_src(const char *src, const char *little, size_t t)
{
	size_t	k;

	k = 0;
	while (little[k] != '\0')
	{
		if (src[t + k] != little[k])
			return (0);
		k++;
	}
	return (1);
}

char	*ft_strnstr(const char *src, const char *little, size_t n)
{
	size_t	i;
	size_t	lenl;
	int		j;
	char	*t;

	i = 0;
	lenl = ft_strlen(little);
	t = (char *)src;
	if (little[0] == '\0')
		return (t);
	if (src[0] == '\0')
		return (NULL);
	while (i + lenl <= n)
	{
		if (t[i] == little[0])
		{
			j = is_in_src(src, little, i);
			if (j == 1)
				return (&t[i]);
		}
		i++;
	}
	return (NULL);
}
/*
int main()
{
	char *t1 = "lolhahalol";
	char *t2 = "haha";
	size_t max = ft_strlen(t1);

	char *i1 = strnstr(t1, t2, max);
	char *i2 = ft_strnstr(t1, t2, max);

	printf("strnstr : %s\nft_strnstr : %s\n", i1 , i2);
	return (0);
}*/
