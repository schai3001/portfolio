/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:41:57 by schai             #+#    #+#             */
/*   Updated: 2022/11/12 12:23:26 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

static size_t	ft_intlen(int nb)
{
	int	len;

	len = 0;
	if (nb <= 0)
		len++;
	while (nb)
	{
		len++;
		nb = nb / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*nbrchar;
	long	nb;

	len = ft_intlen(n);
	nb = n;
	nbrchar = malloc((len + 1) * sizeof(char));
	if (!nbrchar)
		return (NULL);
	if (nb == 0)
		nbrchar[0] = '0';
	if (nb < 0)
	{
		nbrchar[0] = '-';
		nb = -nb;
	}
	nbrchar[len--] = '\0';
	while (nb)
	{
		nbrchar[len] = (nb % 10) + 48;
		len--;
		nb = nb / 10;
	}
	return (nbrchar);
}
/*
int main(void)
{
	printf("%s\n" , ft_itoa(-12345));
	return (0);
}*/
