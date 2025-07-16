/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 12:04:01 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 16:40:55 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <stdlib.h>

int	ft_len_u(unsigned int n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa_u(unsigned int n)
{
	char	*nb;
	int		len;

	len = ft_len_u(n);
	nb = malloc(sizeof(char) * (len + 1));
	if (!nb)
		return (NULL);
	nb[len] = '\0';
	while (n != 0)
	{
			nb[len - 1] = n % 10 + '0';
			n = n / 10;
			len--;
	}
	return (nb);
}

int	ft_print_u(va_list arg)
{
	unsigned int	n;
	int				len;
	char			*nb;

	n = (unsigned int)va_arg(arg, unsigned int);
	len = 0;
	if (n == 0)
	{
		write (1, "0", 1);
		len += 1;
	}
	else
	{
		nb = ft_itoa_u(n);
		len += ft_strlenint(nb);
		ft_putstr(nb);
		free (nb);
	}
	return (len);
}
