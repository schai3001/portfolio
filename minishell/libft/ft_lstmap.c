/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <schai@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:32:05 by schai             #+#    #+#             */
/*   Updated: 2023/10/10 17:33:15 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION :
	The function ft_lstmap creates a new list from a given list  by 
	applying the function passed as parameter to the original list. If
	the memory allocation fails for any node in the new list, the new list
	will be deleted with the function passed as parameter and its memory
	will be freed.

	RETURN VALUE :
	The new list containing the new values if a functon was provided.
	A new copy of the list if no function was provided.
	NULL if the memory allocation failed.
*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*node;

	if (!lst)
		return (NULL);
	newlst = NULL;
	node = NULL;
	while (lst)
	{
		if (!f)
			node = ft_lstnew(lst->content);
		else
			node = ft_lstnew(f(lst->content));
		if (!node)
		{
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, node);
		lst = lst->next;
	}
	return (newlst);
}
