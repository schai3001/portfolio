/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:35:18 by schai             #+#    #+#             */
/*   Updated: 2022/11/11 18:06:13 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

int	ft_atoi(const char *str)
{
	unsigned int	minus;
	unsigned int	n;

	minus = 0;
	n = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus++;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str - '0');
		str++;
	}
	if (minus == 1)
		n = n * (-1);
	return (n);
}
/*
int main(void)
{
	char s[] = " ---+--+1234ab567";
	int n = ft_atoi(s);
	printf ("%d \n", n);
	return (0);
}
*/
