/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:32:05 by schai             #+#    #+#             */
/*   Updated: 2023/10/10 17:32:40 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION :
	The function ft_isalnum checks whether the value of c is alphanumeric.

	RETURN VALUE :
	Non-zero if c is alphanumeric, zero if not.
*/

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (c);
	return (0);
}
