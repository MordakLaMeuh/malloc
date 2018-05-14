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

struct s_couple {
	size_t	len;
	void	*addr;
};

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
		out->len += (uint64_t)record->content - (uint64_t)prev->content
				- (uint64_t)prev->size;
		out->addr = (void *)((uint64_t)prev->content + (uint64_t)prev->size);
		s->addr = (void *)((uint64_t)prev->content + (uint64_t)prev->size);
		s->len = (size_t)record->content - (size_t)prev->content - prev->size;
	}
	else if (record->content != (void *)index->size)
	{
		out->len += (uint64_t)record->content - (uint64_t)index->size;
		out->addr = (void *)index->size;
		s->addr = (void *)((uint64_t)index->size);
		s->len = (uint64_t)record->content - index->size;
	}
}

int		apply_modif(
		struct s_node *record,
		struct s_node *index,
		struct s_couple s[2],
		enum e_page_type type)
{
	btree_delete_rnb_node((struct s_node **)&index->content,
			record, &node_custom_deallocator);
	if (s[0].len > 0)
		fflush_neighbours(s[0].len, s[0].addr, type);
	if (s[1].len > 0)
		fflush_neighbours(s[1].len, s[1].addr, type);
	return (0);
}

int		inherit_neighbour(
		struct s_node *record,
		struct s_node *index,
		struct s_couple *out,
		enum e_page_type type)
{
	struct s_node *next;
	struct s_couple s[2];

	s[0].len = 0;
	s[1].len = 0;
	if ((next = btree_get_next_neighbours_node(record)) != NULL)
	{
		out->len = (uint64_t)next->content - (uint64_t)record->content;
		s[0].addr = (void *)((uint64_t)record->content +
				(uint64_t)record->size);
		s[0].len = out->len - record->size;
	}
	else if ((uint64_t)record->content +
			record->size != index->size + ((type == TINY) ?
			TINY_RANGE : MEDIUM_RANGE))
	{
		s[0].len = (uint64_t)index->size + ((type == TINY) ?
				TINY_RANGE : MEDIUM_RANGE) - ((uint64_t)record->content
						+ record->size);
		out->len = s[0].len + record->size;
		s[0].addr = (void *)((uint64_t)record->content +
				(uint64_t)record->size);
	}
	do_prev_job(out, &s[1], record, index);
	return (apply_modif(record, index, s, type));
}

void	tiny_medium_deallocate(
		struct s_node *record,
		struct s_node *index,
		enum e_page_type type)
{
	struct s_couple s;

	s.addr = record->content;
	s.len = record->size;

	inherit_neighbour(record, index, &s, type);
	insert_free_record(s.addr, s.len, type, NULL);
}

void	destroy_large_page(struct s_node *record)
{
	destroy_pages(record->content, record->size);
	btree_delete_rnb_node(&ctx.big_page_record_tree,
			record, &node_custom_deallocator);
	return ;
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
			ctx.tiny_index_pages_tree, ptr,
			cmp_addr_to_node_size_tiny_range);
	if (record == NULL && index == NULL)
		index = (struct s_node *)btree_get_node_by_content(
			ctx.medium_index_pages_tree, ptr,
			cmp_addr_to_node_size_medium_range);
	if (record == NULL)
		record = btree_get_node_by_content(index->content, ptr,
			&cmp_addr_to_node_addr);
	if (record)
		ft_printf("{magenta}Founded ! addr: %p size: %lu{eoc}\n", record->content, record->size);
	else
		ft_printf("{magenta}not found !{eoc}\n");
	if (record == NULL)
		return ;
	type = get_page_type(record->size);
	if (type == LARGE)
		destroy_large_page(record);
	else
		tiny_medium_deallocate(record, index, type);
}
