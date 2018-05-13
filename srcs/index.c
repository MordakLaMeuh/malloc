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

void		**find_index_node(
		void *addr,
		enum e_node_type type)
{
	struct s_node *index;

	index = (struct s_node *)btree_get_node_by_content(
			(type == RECORD_ALLOCATED_TINY) ?
			ctx.tiny_index_pages_tree : ctx.medium_index_pages_tree,
			addr,
			(type == RECORD_ALLOCATED_TINY) ?
					&cmp_addr_to_node_size_tiny_range :
					&cmp_addr_to_node_size_medium_range);
	if (index == NULL)
		return (NULL);
	return (&index->content);
}

struct s_node	*__find_index_node(
		void *addr,
		enum e_page_type type)
{
	struct s_node *index;

	index = (struct s_node *)btree_get_node_by_content(
			(type == TINY) ?
			ctx.tiny_index_pages_tree : ctx.medium_index_pages_tree,
			addr,
			(type == TINY) ?
					&cmp_addr_to_node_size_tiny_range :
					&cmp_addr_to_node_size_medium_range);
	return (index);
}

void		*insert_allocated_record(
		struct s_node *record,
		enum e_page_type type)
{
	struct s_node *index;

	index = __find_index_node(record->content, type);
	record = btree_insert_rnb_node((struct s_node **)&index->content, record, cmp_node_addr_to_node_addr);
	assert(record != NULL);
	if (record == NULL)
		return (NULL);
	return (record->content);
}

void		*create_index(
		void *addr,
		enum e_page_type type)
{
	struct s_node *index;

	index = btree_create_node(&node_custom_allocator);
	if (index == NULL)
		return (NULL);
	index->content = btree_new();
	index->size = (uint64_t)addr;
	if (type == TINY)
	{
		index->mask.s.node_type = INDEX_TINY;
		index = btree_insert_rnb_node(
				&ctx.tiny_index_pages_tree,
				index,
				&cmp_node_size_to_node_size);
	}
	else
	{
		index->mask.s.node_type = INDEX_MEDIUM;
		index = btree_insert_rnb_node(
				&ctx.medium_index_pages_tree,
				index,
				&cmp_node_size_to_node_size);
	}
	return (index);
}

void		destroy_index(
		struct s_node *index,
		enum e_page_type type)
{
	if (type == TINY)
	{
		btree_delete_rnb_node(
				&ctx.tiny_index_pages_tree,
				index,
				&node_custom_deallocator);
	}
	else
	{
		btree_delete_rnb_node(
				&ctx.medium_index_pages_tree,
				index,
				&node_custom_deallocator);
	}
}
