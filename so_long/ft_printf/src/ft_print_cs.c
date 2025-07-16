/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_cs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:42:02 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 17:16:10 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "libft.h"

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write (1, &str[i], 1);
		i++;
	}
}

int	ft_strlenint(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_print_c(va_list arg)
{
	int	c;

	c = (int)va_arg(arg, int);
	write(1, &c, 1);
	return (1);
}

int	ft_print_s(va_list arg)
{
	int		i;
	char	*s;

	s = (char *)va_arg(arg, char *);
	if (s == NULL)
	{
		ft_putstr("(null)");
		return (6);
	}
	else
	{
		ft_putstr(s);
		i = ft_strlenint(s);
		return (i);
	}
}
