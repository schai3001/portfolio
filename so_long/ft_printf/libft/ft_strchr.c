/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:54:23 by schai             #+#    #+#             */
/*   Updated: 2022/11/11 17:56:31 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

char	*ft_strchr(const char *str, int n)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (char)n)
			return ((char *)(str + i));
		i++;
	}
	if (n == 0)
		return ((char *)(str + i));
	return (0);
}
/*
int main()
{
	char a[50] = "Hello world";
	printf("%s\n", ft_strchr(a, 'w'));
	printf("%s\n", ft_strchr(a, 'z'));
	return 0;
}*/
