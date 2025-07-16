/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:13:09 by schai             #+#    #+#             */
/*   Updated: 2022/11/28 13:05:57 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "libft.h"

void	*ft_calloc(size_t nbr, size_t size)
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
	ft_bzero(t, (nbr * size));
	return (t);
}
/*
int main () {
   int i, n;
   int *a;

   printf("Number of elements to be entered:");
   scanf("%d",&n);

   a = (int*)calloc(n, sizeof(int));
   printf("Enter %d numbers:\n",n);
   for( i=0 ; i < n ; i++ ) {
      scanf("%d",&a[i]);
   }

   printf("The numbers entered are: ");
   for( i=0 ; i < n ; i++ ) {
      printf("%d ",a[i]);
   }
   free( a );

   return(0);
}*/
