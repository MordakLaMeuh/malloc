/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size_conversio                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

size_t	allign_size(size_t size, enum e_page_type page_type)
{
	if (page_type == TINY)
	{
		return (((size >> TINY_SHR) +
			((size & TINY_MASK) ? 1 : 0)) << TINY_SHR);
	}
	else if (page_type == MEDIUM)
	{
		return (((size >> MEDIUM_SHR) +
			((size & MEDIUM_MASK) ? 1 : 0)) << MEDIUM_SHR);
	}
	return (((size / ctx.page_size) +
		((size % ctx.page_size) ? 1 : 0)) * ctx.page_size);
}
