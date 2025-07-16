/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:49:03 by schai             #+#    #+#             */
/*   Updated: 2022/11/11 17:42:28 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char	*dstcpy;
	unsigned char	*srccpy;
	size_t			i;

	if (n == 0 || dst == src)
		return (dst);
	i = 0;
	dstcpy = dst;
	srccpy = (unsigned char *)src;
	if (dst < src)
	{
		while (i++ < n)
			*dstcpy++ = *srccpy++;
	}
	else
	{
		while (i < n)
		{
			dstcpy[n - i - 1] = srccpy[n - i - 1];
			i++;
		}
	}
	return (dst);
}
/*
int main () {
   char dest[] = "oldstring";
   const char src[]  = "newstring";

   printf("Before memmove dest = %s, src = %s\n", dest, src);
   ft_memmove(dest, src, 9);
   printf("After memmove dest = %s, src = %s\n", dest, src);

   return(0);
}*/
