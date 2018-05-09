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

static struct s_node	*assign_new_tiny_chunk(
		struct s_record *record,
		struct s_index *index_ref)
{
	ft_printf("Assigning new TINY chunk\n");
	record->size = TINY_RANGE;
	add_free_record(record, TINY);
	return (btree_insert_rnb_node_by_content(&ctx.tiny_index_pages_tree,
			index_ref, &cmp_index_addr, &node_custom_allocator));
}

static struct s_node	*assign_new_medium_chunk(
		struct s_record *record,
		struct s_index *index_ref)
{
	ft_printf("Assigning new MEDIUM chunk\n");
	record->size = MEDIUM_RANGE;
	add_free_record(record, MEDIUM);
	return (btree_insert_rnb_node_by_content(&ctx.medium_index_pages_tree,
			index_ref, &cmp_index_addr, &node_custom_allocator));
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
	index_ref->type = page_type;
	record = record_custom_allocator();
	if (record == NULL)
		return (-1);
	record->addr = (uint64_t)addr;
	if (index_ref->type == TINY)
		node = assign_new_tiny_chunk(record, index_ref);
	else
		node = assign_new_medium_chunk(record, index_ref);
	return ((node == NULL) ? -1 : 0);
}
