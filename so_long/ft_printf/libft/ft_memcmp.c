/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:09:37 by schai             #+#    #+#             */
/*   Updated: 2022/11/11 17:49:59 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

int	ft_memcmp(const void *p1, const void *p2, size_t size)
{
	unsigned char	*c1;
	unsigned char	*c2;
	size_t			i;

	i = 0;
	c1 = (unsigned char *)p1;
	c2 = (unsigned char *)p2;
	while (i < size)
	{
		if (c1[i] != c2[i])
			return (c1[i] - c2[i]);
		i++;
	}
	return (0);
}
/*
int main()
{
	int r = 0;
	char s1[15] = "Hello";
	char s2[15] = "Hahaouasi;
	r = ft_memcmp(s1, s2, 5);
	printf("%d\n", r);
	return (0);
}*/
