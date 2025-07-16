/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:52:41 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 18:36:09 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "libft.h"

int	ft_plen(uintptr_t n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		len++;
		n = n / 16;
	}
	return (len);
}

void	ft_putp(uintptr_t n)
{
	if (n >= 16)
	{
		ft_putp(n / 16);
		ft_putp(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + '0'), 1);
		else
			ft_putchar_fd((n - 10 + 'a'), 1);
	}
}

int	ft_print_p(va_list arg)
{
	int		i;
	t_ull	p;

	i = 0;
	p = (t_ull)va_arg(arg, t_ull);
	if (p == 0)
	{
		write (1, "(nil)", 5);
		i += 5;
	}
	else
	{
		write (1, "0x", 2);
		i += 2;
		ft_putp(p);
		i += ft_plen(p);
	}
	return (i);
}
