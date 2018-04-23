/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 17:50:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

void			ft_free(void *ptr)
{
	ft_putstr("custom free called !\n");
	(void)ptr;
}

void			*ft_realloc(void *ptr, size_t size)
{
	ft_putstr("custom realoc called !\n");
	(void)ptr;
	(void)size;
	return (NULL);
}

void			*ft_calloc(size_t count, size_t size)
{
	ft_putstr("custom calloc called !\n");
	(void)size;
	(void)count;
	return (NULL);
}
