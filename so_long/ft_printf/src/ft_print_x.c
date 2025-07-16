/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 12:39:52 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 14:56:17 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "libft.h"

int	ft_len_hex(unsigned int n)
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

void	ft_put_hex(unsigned int n)
{
	if (n >= 16)
	{
		ft_put_hex(n / 16);
		ft_put_hex(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + '0'), 1);
		else
			ft_putchar_fd((n - 10 + 'a'), 1);
	}
}

void	ft_put_hex_maj(unsigned int n)
{
	if (n >= 16)
	{
		ft_put_hex_maj(n / 16);
		ft_put_hex_maj(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + '0'), 1);
		else
			ft_putchar_fd((n - 10 + 'A'), 1);
	}
}

int	ft_print_x_maj(va_list arg)
{
	unsigned int	n;

	n = (unsigned int)va_arg(arg, unsigned int);
	if (n == 0)
	{
		write (1, "0", 1);
		return (1);
	}
	else
		ft_put_hex_maj(n);
	return (ft_len_hex(n));
}

int	ft_print_x(va_list arg)
{
	unsigned int	n;

	n = (unsigned int)va_arg(arg, unsigned int);
	if (n == 0)
	{
		write (1, "0", 1);
		return (1);
	}
	else
		ft_put_hex(n);
	return (ft_len_hex(n));
}
