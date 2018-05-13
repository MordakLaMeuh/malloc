/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
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

	ft_printf("searching neighbours: %lu at %p\n", len, address);
	node = get_free_record(address, len, &parent, type);
	assert(node != NULL);
	delete_free_record(node, parent, type);
}

int		inherit_neighbour(
		struct s_node *record,
		struct s_node *index,
		void **addr,
		size_t *size,
		enum e_page_type type)
{
	struct s_node *next;
	struct s_node *prev;
	size_t len[2];
	void *address[2];

	len[0] = 0;
	len[1] = 0;
	if ((next = btree_get_next_neighbours_node(record)) != NULL)
	{
		*size = (uint64_t)next->content - (uint64_t)record->content;
		address[0] = (void *)((uint64_t)record->content + (uint64_t)record->size);
		len[0] = *size - record->size;
	}
	if ((prev = btree_get_prev_neighbours_node(record)) != NULL)
	{
		*size += (uint64_t)record->content - (uint64_t)prev->content
				- (uint64_t)prev->size;
		*addr = (void *)((uint64_t)prev->content + (uint64_t)prev->size);
		address[1] = (void *)((uint64_t)prev->content + (uint64_t)prev->size);
		len[1] = (size_t)record->content - (size_t)prev->content - prev->size;
	}
	if (next == NULL && ((uint64_t)record->content + record->size != index->size + ((type == TINY) ? TINY_RANGE : MEDIUM_RANGE)))
	{
		len[0] = (uint64_t)index->size + ((type == TINY) ?
				TINY_RANGE : MEDIUM_RANGE) - ((uint64_t)record->content + record->size);
		*size = len[0] + record->size;
		address[0] = (void *)((uint64_t)record->content + (uint64_t)record->size);
	}
	if (prev == NULL && record->content != (void *)index->size)
	{
		*size += (uint64_t)record->content - (uint64_t)index->size;
		*addr = (void *)index->size;
		address[1] = (void *)((uint64_t)index->size);
		len[1] = (uint64_t)record->content - index->size;
	}

	btree_delete_rnb_node((struct s_node **)&index->content, record, &node_custom_deallocator);
	if (len[0] > 0)
		fflush_neighbours(len[0], address[0], type);
	if (len[1] > 0)
		fflush_neighbours(len[1], address[1], type);
	return (0);
}

void	tiny_medium_deallocate(
		struct s_node *record,
		struct s_node *index,
		enum e_page_type type)
{
	void *addr;
	size_t size;

	addr = record->content;
	size = record->size;

	inherit_neighbour(record, index, &addr, &size, type);
	insert_free_record(addr, size, type, NULL);
}

void	core_deallocator(void *ptr)
{
	struct s_node *record;
	struct s_node *index;
	enum e_page_type type;

	index = NULL;
	record = btree_get_node_by_content(ctx.big_page_record_tree,
			ptr, &cmp_addr_to_node_addr);
	if (record == NULL)
		index = (struct s_node *)btree_get_node_by_content(
			ctx.tiny_index_pages_tree,
			ptr,
			cmp_addr_to_node_size_tiny_range);
	if (record == NULL && index == NULL)
		index = (struct s_node *)btree_get_node_by_content(
			ctx.medium_index_pages_tree,
			ptr,
			cmp_addr_to_node_size_medium_range);
	if (record == NULL)
		record = btree_get_node_by_content(index->content, ptr, &cmp_addr_to_node_addr);

	if (record)
		ft_printf("{magenta}Founded ! addr: %p size: %lu{eoc}\n", record->content, record->size);
	else
	{
		ft_printf("{magenta}not found !{eoc}\n");
		return ;
	}

	type = get_page_type(record->size);
	if (type == LARGE)
	{
		destroy_pages(record->content, record->size);
		btree_delete_rnb_node(&ctx.big_page_record_tree,
				record, &node_custom_deallocator);
		return ;
	}
	tiny_medium_deallocate(record, index, type);
}
