/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static int		cmp_size(void *r1, void *r2)
{
	if (((struct s_record *)r1)->size < ((struct s_record *)r2)->size)
		return (-1);
	if (((struct s_record *)r1)->size > ((struct s_record *)r2)->size)
		return (1);
	return (0);
}

static int		cmp_addr(void *r1, void *r2)
{
	if (((struct s_index *)r1)->page_addr < ((struct s_index *)r2)->page_addr)
		return (-1);
	if (((struct s_index *)r1)->page_addr > ((struct s_index *)r2)->page_addr)
		return (1);
	return (0);
}

int				assign_new_chunk(void *addr, enum e_page_type page_type)
{
	struct s_index		*index_ref;
	struct s_record		*record;
	struct s_node		*node;

	index_ref = index_custom_allocator();
	if (index_ref == NULL)
		return (-1);
	index_ref->page_addr = (uint64_t)addr;
	index_ref->allocation_tree = btree_new();
	index_ref->space_tree = btree_new();
	index_ref->type = page_type;
	record = record_custom_allocator();
	if (record == NULL)
		return (-1);
	record->addr = (uint64_t)addr;
	record->size = (index_ref->type == TINY) ? TINY_RANGE : MEDIUM_RANGE;
	node = btree_insert_rnb_node_by_content(&index_ref->space_tree, record,
			&cmp_size, &node_custom_allocator);
	if (node == NULL)
		return (-1);
	node = btree_insert_rnb_node_by_content(index_ref->type == TINY ?
			&ctx.tiny_index_pages_tree : &ctx.medium_index_pages_tree,
			index_ref, &cmp_addr, &node_custom_allocator);
	return ((node == NULL) ? -1 : 0);
}
