/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfmain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:28:19 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 17:14:58 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "libft.h"
#include <stdarg.h>
#include <stdio.h>

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	int		nbcarac;

	va_start(arg, format);
	nbcarac = check_nb(format, arg);
	va_end(arg);
	return (nbcarac);
}

int	check_nb(const char *format, va_list arg)
{
	int	i;
	int	len;

	if (format == 0)
		return (-1);
	i = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			len = check_format(format, arg);
			if (len == -1)
				return (-1);
			i += len;
		}
		else
		{
			if (write(1, format, 1) == -1)
				return (-1);
			i++;
		}
		format++;
	}
	return (i);
}

int	check_format(const char *format, va_list arg)
{
	if (*format == 'c')
		return (ft_print_c(arg));
	else if (*format == 's')
		return (ft_print_s(arg));
	else if (*format == 'p')
		return (ft_print_p(arg));
	else if (*format == 'd' || *format == 'i')
		return (ft_print_di(arg));
	else if (*format == 'u')
		return (ft_print_u(arg));
	else if (*format == 'x')
		return (ft_print_x(arg));
	else if (*format == 'X')
		return (ft_print_x_maj(arg));
	else if (*format == '%')
	{
		write(1, "%", 1);
		return (1);
	}
	else
		return (-1);
}
/*
int	main()
{
	char *s = "haha";
	int d = 10;
	char *s2 = "lol";

	printf("%d\n%s\n%s", d, s, s2);
	return (0);
}*/
