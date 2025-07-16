/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:30:27 by schai             #+#    #+#             */
/*   Updated: 2022/11/09 17:19:32 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>

void	*ft_memchr(const void *m, int n, size_t size)
{
	unsigned char	*p;
	unsigned char	*t;

	t = NULL;
	p = (unsigned char *)m;
	while ((m != NULL) && (size--))
	{
		if (*p != (unsigned char)n)
		{
			p++;
		}
		else
		{
			t = p;
			break ;
		}
	}
	return (t);
}
/*
#include <stdio.h>
#include <string.h>

int main () 
{
   const char str[] = "http://www.tutorialspoint.com";
   const char ch = '.';
   char *ret;

   ret = ft_memchr(str, ch, strlen(str));

   printf("String after |%c| is - |%s|\n", ch, ret);

   return(0);
}*/
