/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deallocator_next.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

/*
** Search in index pages
** Search in allocated records		+1 node
** Search neighbours left			if exist +1 node
** Search neighbours right			if exist +1 node
** Insert free record				Can take 2 nodes
*/

void	fflush_neighbours(
		size_t len,
		void *address,
		enum e_page_type type)
{
	struct s_node *node;
	struct s_node *parent;

	node = get_free_record(address, len, &parent, type);
	delete_free_record(node, parent, type);
}

void	do_prev_job(
		struct s_couple *out,
		struct s_couple *s,
		struct s_node *record,
		struct s_node *index)
{
	struct s_node *prev;

	prev = btree_get_prev_neighbours_node(record);
	if (prev != NULL)
	{
		out->len += (uint64_t)record->ptr_a - (uint64_t)prev->ptr_a
				- (uint64_t)prev->m.size;
		out->addr = (void *)((uint64_t)prev->ptr_a + (uint64_t)prev->m.size);
		s->addr = (void *)((uint64_t)prev->ptr_a + (uint64_t)prev->m.size);
		s->len = (size_t)record->ptr_a - (size_t)prev->ptr_a - prev->m.size;
	}
	else if (record->ptr_a != (void *)index->m.size)
	{
		out->len += (uint64_t)record->ptr_a - (uint64_t)index->m.size;
		out->addr = (void *)index->m.size;
		s->addr = (void *)((uint64_t)index->m.size);
		s->len = (uint64_t)record->ptr_a - index->m.size;
	}
}
