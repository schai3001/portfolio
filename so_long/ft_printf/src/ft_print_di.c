/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_di.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:49:48 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 18:20:51 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "libft.h"

int	ft_print_di(va_list arg)
{
	int		n;
	int		len;
	char	*nb;

	n = (int)va_arg(arg, int);
	nb = ft_itoa(n);
	ft_putstr(nb);
	len = ft_strlenint(nb);
	free (nb);
	return (len);
}
