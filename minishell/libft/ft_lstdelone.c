/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:32:05 by schai             #+#    #+#             */
/*   Updated: 2023/10/10 17:33:08 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION :
	The function ft_lstdelone deletes the content of a list node with the
	function passed as parameter before freeing the memory of the node.

	RETURN VALUE :
	None.
*/

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (del)
		(del)(lst->content);
	free(lst);
}
