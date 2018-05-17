/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocator.next.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

void	*substract_large_page(
		struct s_node *record,
		size_t new_size)
{
	uint8_t		*cut_point;
	size_t		cut_size;

	ctx.size_owned_by_data -= record->m.size;
	cut_point = (uint8_t *)record->ptr_a + new_size;
	cut_size = record->m.size - new_size;
	destroy_pages((void *)cut_point, cut_size);
	record->m.size = new_size;
	ctx.size_owned_by_data += record->m.size;
	return (record->ptr_a);
}
