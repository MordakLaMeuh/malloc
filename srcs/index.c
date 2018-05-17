/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

void		**find_index_node(void *addr)
{
	struct s_node *index;

	index = (struct s_node *)btree_get_node_by_content(
			ctx.index_pages_tree,
			addr,
			&cmp_addr_to_node_m_addr_range);
	if (index == NULL)
		return (NULL);
	return (&index->ptr_a);
}

void		*insert_allocated_record(struct s_node *record)
{
	struct s_node *index;

	index = (struct s_node *)btree_get_node_by_content(
			ctx.index_pages_tree,
			record->ptr_a,
			&cmp_addr_to_node_m_addr_range);
	record = btree_insert_rnb_node((struct s_node **)&index->ptr_a,
			record, cmp_node_addr_to_node_addr);
	assert(record != NULL);
	if (record == NULL)
		return (NULL);
	return (record->ptr_a);
}

void		*create_index(
		void *addr,
		uint32_t range)
{
	struct s_node *index;

	index = btree_create_node(&node_custom_allocator);
	if (index == NULL)
		return (NULL);
	index->ptr_a = btree_new();
	index->m.ptr_b = addr;
	index->mask.s.range = range;
	index->mask.s.node_type = INDEX;
	index = btree_insert_rnb_node(
			&ctx.index_pages_tree,
			index,
			&cmp_node_m_addr_to_node_m_addr);
	return (index);
}

void		destroy_index(struct s_node *index)
{
	btree_delete_rnb_node(
			&ctx.index_pages_tree,
			index,
			&node_custom_deallocator);
}
